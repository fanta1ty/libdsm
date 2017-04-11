//
//  md5.h
//  test
//
//  Created by trekvn on 4/10/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef md5_h
#define md5_h

#include <stdio.h>

/* Any 32-bit or wider unsigned integer data type will do */
typedef unsigned int MD5_u32plus;

typedef struct
{
    MD5_u32plus lo, hi;
    MD5_u32plus a, b, c, d;
    unsigned char buffer[64];
    MD5_u32plus block[16];
} MD5_CTX;

extern void MD5_Init(MD5_CTX *ctx);
extern void MD5_Update(MD5_CTX *ctx, const void *data, unsigned long size);
extern void MD5_Final(unsigned char *result, MD5_CTX *ctx);


#endif /* md5_h */
