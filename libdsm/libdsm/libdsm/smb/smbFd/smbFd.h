//
//  smbFd.h
//  test
//
//  Created by trekvn on 4/13/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import "smbHeader.h"

@interface smbFd : NSObject
#pragma mark - smbSessionShareAdd
void smb_session_share_add(smb_session *s, smb_share *share);

#pragma mark - smbSessionShareGet
smb_share *smb_session_share_get(smb_session *s, smb_tid tid);

#pragma mark - smbSessionShareRemove
smb_share *smb_session_share_remove(smb_session *s, smb_tid tid);

#pragma mark - smbSessionShareClear
void smb_session_share_clear(smb_session *s);

#pragma mark - smbSessionFileAdd
int smb_session_file_add(smb_session *s, smb_tid tid, smb_file *f);

#pragma mark - smbSessionFileGet
smb_file *smb_session_file_get(smb_session *s, smb_fd fd);

#pragma mark - smbSessionFileRemove
smb_file *smb_session_file_remove(smb_session *s, smb_fd fd);
@end
#endif
