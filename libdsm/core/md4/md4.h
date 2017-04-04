//
//  md4.h
//  libdsm
//
//  Created by Thinh Nguyen Duc on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef md4_h
#define md4_h

#include <stdio.h>

/* Any 32-bit or wider unsigned integer data type will do */
typedef unsigned int MD4_u32plus;

typedef struct {
    MD4_u32plus     lo, hi;
    MD4_u32plus     a, b, c, d;
    unsigned char   buffer[64];
    MD4_u32plus     block[16];
} MD4_CTX;

extern void MD4_Init(MD4_CTX *ctx);

extern void MD4_Update(MD4_CTX *ctx, const void *data, unsigned long size);

extern void MD4_Final(unsigned char *result, MD4_CTX *ctx);

#endif /* md4_h */
