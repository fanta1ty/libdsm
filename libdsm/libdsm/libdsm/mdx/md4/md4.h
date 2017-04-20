//
//  md4.h
//  test
//
//  Created by trekvn on 4/13/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>
typedef unsigned int MD4_u32plus;

typedef struct {
    MD4_u32plus lo, hi;
    MD4_u32plus a, b, c, d;
    unsigned char buffer[64];
    MD4_u32plus block[16];
} MD4_CTX;

@interface md4 : NSObject
#pragma mark - md4Init
extern void MD4_Init(MD4_CTX *ctx);

#pragma mark - md4Update
extern void MD4_Update(MD4_CTX *ctx, const void *data, unsigned long size);

#pragma mark - md4Final
extern void MD4_Final(unsigned char *result, MD4_CTX *ctx);
@end
#endif
