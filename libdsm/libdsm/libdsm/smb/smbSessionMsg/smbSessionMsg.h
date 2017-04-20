//
//  smb_session_msg.h
//  test
//
//  Created by trekvn on 4/12/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>

#import "smbHeader.h"

@interface smbSessionMsg : NSObject
#pragma mark - smbSessionSendMessage
/*!Send a smb message for the provided smb_session
 */
int smb_session_send_msg(smb_session *s, smb_message *msg);

#pragma mark - smbSessionRecvMessage
/*!msg->packet will be updated to point on received data. You don't own this memory. It'll be reused on next recv_msg
 */
size_t smb_session_recv_msg(smb_session *s, smb_message *msg);
@end
#endif
