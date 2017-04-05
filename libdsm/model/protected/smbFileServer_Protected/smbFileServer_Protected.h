//
//  smbFileServer_Protected.h
//  libdsm
//
//  Created by trekvn on 4/5/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import "smbFileServer.h"
#import "smb_session.h"

@interface smbFileServer ()
@property (nonatomic, assign, readonly, nullable) smb_session *smbSession;

- (void)openShare:(nonnull NSString *)name
       completion:(nullable void (^)(smb_tid tid, NSError * _Nullable error))completion;

- (void)closeShare:(smb_tid)shareID
        completion:(nullable void (^)(NSError * _Nullable error))completion;
@end
