//
//  smb_session_msg.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef smb_session_msg_h
#define smb_session_msg_h

#include <stdio.h>
#include "smb_types.h"

/*!Send a smb message for the provided smb_session
 */
int smb_session_send_msg(smb_session *s, smb_message *msg);

/*!msg->packet will be updated to point on received data. 
 * You don't own this memory. It'll be reused on next recv_msg
 */
size_t smb_session_recv_msg(smb_session *s, smb_message *msg);

#endif /* smb_session_msg_h */
