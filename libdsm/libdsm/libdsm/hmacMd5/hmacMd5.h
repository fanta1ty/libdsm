//
//  hmac_md5.h
//  test
//
//  Created by Thinh Nguyen Duc on 4/11/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifdef __OBJC__
#import <Foundation/Foundation.h>
@interface hmacMd5 : NSObject

unsigned char *HMAC_MD5(const void *key, size_t key_len, const void *msg,
                        size_t msg_len, void *hmac);
@end
#endif
