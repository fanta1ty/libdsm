//
//  smb_fd.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef smb_fd_h
#define smb_fd_h

#include <stdio.h>

#include "smb_session.h"
#include "smb_message.h"

void smb_session_share_add(smb_session *s, smb_share *share);

smb_share *smb_session_share_get(smb_session *s, smb_tid tid);

smb_share *smb_session_share_remove(smb_session *s, smb_tid tid);

void smb_session_share_clear(smb_session *s);

int smb_session_file_add(smb_session *s, smb_tid tid, smb_file *f);

smb_file *smb_session_file_get(smb_session *s, smb_fd fd);

smb_file *smb_session_file_remove(smb_session *s, smb_fd fd);

#endif /* smb_fd_h */
