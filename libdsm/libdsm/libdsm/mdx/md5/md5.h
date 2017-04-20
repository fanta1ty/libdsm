//
//  md5.h
//  test
//
//  Created by Thinh Nguyen Duc on 4/11/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>
typedef unsigned int MD5_u32plus;

typedef struct {
    MD5_u32plus lo, hi;
    MD5_u32plus a, b, c, d;
    unsigned char buffer[64];
    MD5_u32plus block[16];
} MD5_CTX;

@interface md5 : NSObject
#pragma mark - md5Init
extern void MD5_Init(MD5_CTX *ctx);

#pragma mark - md5Update
extern void MD5_Update(MD5_CTX *ctx, const void *data, unsigned long size);

#pragma mark - md5Finish
extern void MD5_Final(unsigned char *result, MD5_CTX *ctx);
@end
#endif
