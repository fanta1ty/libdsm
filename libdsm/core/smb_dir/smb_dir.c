//
//  smb_dir.c
//  libdsm
//
//  Created by trekvn on 4/5/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#include "smb_dir.h"
#include "config.h"
#include "smb_session_msg.h"
#include "smb_fd.h"
#include "smb_utils.h"

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int smb_directory_rm(smb_session *s, smb_tid tid, const char *path) {
    smb_message           *req_msg, resp_msg;
    smb_directory_rm_req  req;
    smb_directory_rm_resp *resp;
    size_t                utf_pattern_len;
    char                  *utf_pattern;
    
    assert(s != NULL && path != NULL);
    
    utf_pattern_len = smb_to_utf16(path, strlen(path) + 1, &utf_pattern);
    if (utf_pattern_len == 0)
        return DSM_ERROR_CHARSET;
    
    req_msg = smb_message_new(SMB_CMD_RMDIR);
    if (!req_msg) {
        free(utf_pattern);
        return DSM_ERROR_GENERIC;
    }
    
    req_msg->packet->header.tid = (uint16_t)tid;
    
    SMB_MSG_INIT_PKT(req);
    req.wct              = 0x00; // Must be 0
    req.bct              = (uint16_t)(utf_pattern_len + 1);
    req.buffer_format    = 0x04; // Must be 4
    SMB_MSG_PUT_PKT(req_msg, req);
    smb_message_append(req_msg, utf_pattern, utf_pattern_len);
    
    smb_session_send_msg(s, req_msg);
    smb_message_destroy(req_msg);
    
    free(utf_pattern);
    
    if (!smb_session_recv_msg(s, &resp_msg))
        return DSM_ERROR_NETWORK;
    
    if (!smb_session_check_nt_status(s, &resp_msg))
        return DSM_ERROR_NT;
    
    resp = (smb_directory_rm_resp *)resp_msg.packet->payload;
    if ((resp->wct != 0) || (resp->bct != 0))
        return DSM_ERROR_NETWORK;
    
    return 0;
}

int smb_directory_create(smb_session *s, smb_tid tid, const char *path) {
    smb_message           *req_msg, resp_msg;
    smb_directory_mk_req  req;
    smb_directory_mk_resp *resp;
    size_t                utf_pattern_len;
    char                  *utf_pattern;
    
    assert(s != NULL && path != NULL);
    
    utf_pattern_len = smb_to_utf16(path, strlen(path) + 1, &utf_pattern);
    if (utf_pattern_len == 0)
        return DSM_ERROR_CHARSET;
    
    req_msg = smb_message_new(SMB_CMD_MKDIR);
    if (!req_msg) {
        free(utf_pattern);
        return DSM_ERROR_GENERIC;
    }
    
    req_msg->packet->header.tid = (uint16_t)tid;
    
    SMB_MSG_INIT_PKT(req);
    req.wct              = 0x00; // Must be 0
    req.bct              = (uint16_t)(utf_pattern_len + 1);
    req.buffer_format    = 0x04; // Must be 4
    SMB_MSG_PUT_PKT(req_msg, req);
    smb_message_append(req_msg, utf_pattern, utf_pattern_len);
    
    smb_session_send_msg(s, req_msg);
    smb_message_destroy(req_msg);
    
    free(utf_pattern);
    
    if (!smb_session_recv_msg(s, &resp_msg))
        return DSM_ERROR_NETWORK;
    
    if (!smb_session_check_nt_status(s, &resp_msg))
        return DSM_ERROR_NT;
    
    resp = (smb_directory_mk_resp *)resp_msg.packet->payload;
    if ((resp->wct != 0) || (resp->bct != 0))
        return DSM_ERROR_NETWORK;
    
    return 0;
}
