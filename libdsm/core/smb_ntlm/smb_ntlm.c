//
//  smb_ntlm.c
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#include "smb_ntlm.h"
#include "compat.h"
#include "bdsm_debug.h"
#include "smb_utils.h"

#include <assert.h>
#include <ctype.h>
#include <wctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#ifdef _WIN32
#   define _CRT_RAND_S
#endif

#ifdef HAVE_BSD_STRING_H
#   include <bsd/string.h>
#else
#   include "compat.h"
#endif

uint64_t smb_ntlm_generate_challenge() {
    
}

void smb_ntlm_generate_xkey(smb_ntlmh cli_session_key) {
    
}

void smb_ntlm_hash(const char *password, smb_ntlmh hash) {
    
}

void smb_ntlm2_hash(const char *username, const char *password,
                    const char *destination, smb_ntlmh hash) {
    
}

size_t smb_ntlm_make_blob(smb_ntlm_blob **blob, uint64_t ts,
                          uint64_t user_challenge, smb_buffer *target) {
    
}

uint8_t *smb_ntlm2_response(smb_ntlmh hash_v2, uint64_t srv_challenge,
                            smb_buffer *blob) {
    
}

uint8_t *smb_lm2_response(smb_ntlmh hash_v2, uint64_t srv_challenge,
                          uint64_t user_challenge) {
    
}

void smb_ntlm2_session_key(smb_ntlmh hash_v2, void *ntlm2,
                           smb_ntlmh xkey, smb_ntlmh enc_xkey) {
    
}

void smb_ntlmssp_negotiate(const char *host, const char *domain,
                           smb_buffer *token) {
    
}

void smb_ntlmssp_response(uint64_t srv_challenge, uint64_t srv_ts,
                          const char *host, const char *domain,
                          const char *user, const char *password,
                          smb_buffer *target, smb_buffer *token) {
    
}
