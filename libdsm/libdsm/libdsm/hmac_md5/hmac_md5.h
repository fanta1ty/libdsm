//
//  hmac_md5.h
//  test
//
//  Created by trekvn on 4/10/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef hmac_md5_h
#define hmac_md5_h

#include <stdio.h>
#include <stdint.h>
#include "md5.h"

// Pay attention that this is not HMAC_MD5 stricto sensus, this is a variation
// to respect MS non-standard implementation in NTLMv2 auth.
unsigned char *HMAC_MD5(const void *key, size_t key_len, const void *msg,
                        size_t msg_len, void *hmac);
#endif /* hmac_md5_h */
