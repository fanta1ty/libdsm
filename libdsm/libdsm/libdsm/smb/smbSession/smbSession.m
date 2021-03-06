//
//  smbSession.m
//  test
//
//  Created by trekvn on 4/13/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#import <stdlib.h>
#import <string.h>

#import "smbSession.h"
static int smb_negotiate(smb_session *s);

@implementation smbSession
#pragma mark - smb_session_new

smb_session *smb_session_new()
{
    smb_session *s;
    
    s = calloc(1, sizeof(smb_session));
    if (!s)
        return NULL;
    
    s->guest              = false;
    
    // Explicitly sets pointer to NULL, insted of 0
    s->spnego_asn1        = NULL;
    s->transport.session  = NULL;
    s->shares             = NULL;
    
    s->creds.domain       = NULL;
    s->creds.login        = NULL;
    s->creds.password     = NULL;
    
    smb_buffer_init(&s->xsec_target, NULL, 0);
    
    // Until we know more, assume server supports everything.
    // s->c
    
    return s;
}

#pragma mark - smbSessionDestroy
void smb_session_destroy(smb_session *s)
{
    assert(s != NULL);
    
    smb_session_share_clear(s);
    
    // FIXME Free smb_share and smb_file
    if (s->transport.session != NULL)
    {
        s->transport.destroy(s->transport.session);
        s->transport.session = NULL;
    }
    
    if (s->spnego_asn1 != NULL)
        asn1_delete_structure(&s->spnego_asn1);
    
    smb_buffer_free(&s->xsec_target);
    
    // Free stored credentials.
    free(s->creds.domain);
    free(s->creds.login);
    free(s->creds.password);
    free(s);
}

#pragma mark - smbSessionSetCreds
void smb_session_set_creds(smb_session *s, const char *domain,
                                      const char *login, const char *password)
{
    assert(s != NULL);
    
    if (domain != NULL)
    {
        if (s->creds.domain != NULL)
            free(s->creds.domain);
        s->creds.domain = strndup(domain, SMB_CREDS_MAXLEN);
    }
    
    if (login != NULL)
    {
        if (s->creds.login != NULL)
            free(s->creds.login);
        s->creds.login = strndup(login, SMB_CREDS_MAXLEN);
    }
    
    if (password != NULL)
    {
        if (s->creds.password != NULL)
            free(s->creds.password);
        s->creds.password = strndup(password, SMB_CREDS_MAXLEN);
    }
}

static int smb_negotiate(smb_session *s)
{
    const char          *dialects[] = SMB_DIALECTS;
    smb_message         *msg = NULL;
    smb_message         answer;
    smb_nego_resp       *nego;
    uint16_t *p_payload_size;
    
    assert(s != NULL);
    
    msg = smb_message_new(SMB_CMD_NEGOTIATE);
    if (!msg)
        return DSM_ERROR_GENERIC;
    
    smb_message_put8(msg, 0);   // wct
    smb_message_put16(msg, 0);  // bct, will be updated later
    
    for (unsigned i = 0; dialects[i] != NULL; i++)
        smb_message_append(msg, dialects[i], strlen(dialects[i]) + 1);
    p_payload_size = (uint16_t *)(msg->packet->payload + 1);
    *p_payload_size = msg->cursor - 3;
    
    if (!smb_session_send_msg(s, msg))
    {
        smb_message_destroy(msg);
        return DSM_ERROR_NETWORK;
    }
    smb_message_destroy(msg);
    
    if (!smb_session_recv_msg(s, &answer))
        return DSM_ERROR_NETWORK;
    
    nego = (smb_nego_resp *)answer.packet->payload;
    if (!smb_session_check_nt_status(s, &answer))
        return DSM_ERROR_NT;
    if (nego->wct != 0x11)
        return DSM_ERROR_NETWORK;
    
    s->srv.dialect = nego->dialect_index;
    s->srv.security_mode = nego->security_mode;
    s->srv.caps = nego->caps;
    s->srv.ts = nego->ts;
    s->srv.session_key = nego->session_key;
    
    // Copy SPNEGO supported mechanisms  token for later usage (login_gss())
    if (smb_session_supports(s, SMB_SESSION_XSEC)) {
        
    } else {
        s->srv.challenge = nego->challenge;
    }
    return DSM_SUCCESS;
}

#pragma mark - smbSessionConnect
int smb_session_connect(smb_session *s, const char *name,
                                    uint32_t ip, int transport)
{
    assert(s != NULL && name != NULL);
    
    if (s->transport.session != NULL)
        s->transport.destroy(s->transport.session);
    
    switch (transport)
    {
        case SMB_TRANSPORT_TCP:
            smb_transport_tcp(&s->transport);
            break;
        case SMB_TRANSPORT_NBT:
            smb_transport_nbt(&s->transport);
            break;
        default:
            return DSM_ERROR_GENERIC;
    }
    
    if ((s->transport.session = s->transport.new(SMB_DEFAULT_BUFSIZE)) == NULL)
        return DSM_ERROR_GENERIC;
    if (!s->transport.connect(ip, s->transport.session, name))
        return DSM_ERROR_NETWORK;
    
    memcpy(s->srv.name, name, strlen(name) + 1);
    
    return smb_negotiate(s);
}

static int        smb_session_login_ntlm(smb_session *s, const char *domain,
                                         const char *user, const char *password)
{
    smb_message           answer;
    smb_message           *msg = NULL;
    smb_session_req       req;
    uint8_t               *ntlm2 = NULL;
    smb_ntlmh             hash_v2;
    uint64_t              user_challenge;
    
    assert(s != NULL);
    
    msg = smb_message_new(SMB_CMD_SETUP);
    if (!msg)
        return DSM_ERROR_GENERIC;
    
    // this struct will be set at the end when we know the payload size
    SMB_MSG_ADVANCE_PKT(msg, smb_session_req);
    
    user_challenge = smb_ntlm_generate_challenge();
    
    // LM2 Response
    smb_ntlm2_hash(user, password, domain, hash_v2);
    ntlm2 = smb_lm2_response(hash_v2, s->srv.challenge, user_challenge);
    smb_message_append(msg, ntlm2, 16 + 8);
    free(ntlm2);
    
    if (msg->cursor / 2) // Padding !
        smb_message_put8(msg, 0);
    
    smb_message_put_utf16(msg, user, strlen(user));
    smb_message_put16(msg, 0);
    smb_message_put_utf16(msg, domain, strlen(domain));
    smb_message_put16(msg, 0);
    smb_message_put_utf16(msg, SMB_OS, strlen(SMB_OS));
    smb_message_put16(msg, 0);
    smb_message_put_utf16(msg, SMB_LANMAN, strlen(SMB_LANMAN));
    smb_message_put16(msg, 0);
    
    SMB_MSG_INIT_PKT_ANDX(req);
    req.wct              = 13;
    req.max_buffer       = SMB_SESSION_MAX_BUFFER;
    req.mpx_count        = 16; // XXX ?
    req.vc_count         = 1;
    //req.session_key      = s->srv.session_key; // XXX Useless on the wire?
    req.caps             = s->srv.caps; // XXX caps & our_caps_mask
    req.oem_pass_len = 16 + SMB_LM2_BLOB_SIZE;
    req.uni_pass_len = 0; //16 + blob_size; //SMB_NTLM2_BLOB_SIZE;
    req.payload_size = msg->cursor - sizeof(smb_session_req);
    SMB_MSG_INSERT_PKT(msg, 0, req);
    
    if (!smb_session_send_msg(s, msg)) {
        smb_message_destroy(msg);
        
        return DSM_ERROR_NETWORK;
    }
    smb_message_destroy(msg);
    
    if (smb_session_recv_msg(s, &answer) == 0) {
        return DSM_ERROR_NETWORK;
    }
    
    smb_session_resp *r = (smb_session_resp *)answer.packet->payload;
    if (!smb_session_check_nt_status(s, &answer)) {
        return DSM_ERROR_NT;
    }
    
    if (r->action & 0x0001)
        s->guest = true;
    
    s->srv.uid  = answer.packet->header.uid;
    s->logged = true;
    
    return DSM_SUCCESS;
}

#pragma mark - smbSessionSupports
int smb_session_supports(smb_session *s, int what)
{
    assert(s != NULL);
    
    switch (what)
    {
        case SMB_SESSION_XSEC:
            return s->srv.caps & SMB_CAPS_XSEC;
        default:
            return 0;
    }
}

#pragma mark - smbSessionLogin
int smb_session_login(smb_session *s)
{
    assert(s != NULL);
    
    if (s->creds.domain == NULL
        || s->creds.login == NULL
        || s->creds.password == NULL)
        return DSM_ERROR_GENERIC;
    
    if (smb_session_supports(s, SMB_SESSION_XSEC))
        return (smb_session_login_spnego(s, s->creds.domain, s->creds.login,
                                         s->creds.password));
    else
        return (smb_session_login_ntlm(s, s->creds.domain, s->creds.login,
                                       s->creds.password));
}

#pragma mark - smbSessionIsGuest
int smb_session_is_guest(smb_session *s)
{
    assert(s != NULL);
    
    // We're not logged in yet.
    if (s->logged != true)
        return -1;
    
    // We're logged in as guest
    if (s->guest)
        return 1;
    
    // We're logged in as regular user
    return 0;
}

#pragma mark - smbSessionServerName
const char *smb_session_server_name(smb_session *s)
{
    assert(s != NULL);
    
    return s->srv.name;
}

#pragma mark - smbSessionGetNtStatus
uint32_t smb_session_get_nt_status(smb_session *s)
{
    assert(s != NULL);
    
    return s->nt_status;
}

#pragma mark - smbSessionCheckNtStatus
bool smb_session_check_nt_status(smb_session *s, smb_message *msg)
{
    assert(s != NULL && msg != NULL);
    
    if (msg->packet->header.status != NT_STATUS_SUCCESS)
    {
        s->nt_status = msg->packet->header.status;
        return false;
    }
    return true;
}
@end
