//
//  smbSpnego.h
//  test
//
//  Created by trekvn on 4/13/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import "smbHeader.h"

@interface smbSpnego : NSObject
#pragma mark - smbSessionLoginSpnego
int smb_session_login_spnego(smb_session *s, const char *domain,
                                         const char *user, const char *password);
@end
#endif
