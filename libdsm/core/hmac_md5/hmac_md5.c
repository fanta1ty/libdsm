//
//  hmac_md5.c
//  libdsm
//
//  Created by Thinh Nguyen Duc on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#include "config.h"
#include "hmac_md5.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


unsigned char *HMAC_MD5(const void *key, size_t key_len, const void *msg,
                        size_t msg_len, void *hmac) {
    static uint8_t hmac_static[16];
    
    uint8_t key_pad[64], o_key_pad[64], i_key_pad[64], kcat[80];
    uint8_t *cat, *out;
    MD5_CTX ctx;
    
    assert(key != NULL && msg != NULL);
    
    // This is Microsoft variation of HMAC_MD5 for NTLMv2
    // It seems they truncate over-sized keys instead of rehashing
    if (key_len > 64)
        key_len = 64;
    
    memcpy(key_pad, key, key_len);
    if (key_len < 64)
        memset(key_pad + key_len, 0, 64 - key_len);
    
    // Compute the o/i XORed padded keys
    for (unsigned i = 0; i < 64; i++) {
        o_key_pad[i] = 0x5c ^ key_pad[i];
        i_key_pad[i] = 0x36 ^ key_pad[i];
    }
    
    // Concatenate inner padded key with message
    cat = malloc(msg_len + 64);
    if (!cat)
        return NULL;
    memcpy(cat, i_key_pad, 64);
    memcpy(cat + 64, msg, msg_len);
    
    // MD5 of the result
    MD5_Init(&ctx);
    MD5_Update(&ctx, cat, msg_len + 64);
    MD5_Final(key_pad, &ctx);
    free(cat);
    
    memcpy(kcat, o_key_pad, 64);
    memcpy(kcat + 64, key_pad, 16);
    
    if (hmac != NULL)
        out = hmac;
    else
        out = hmac_static;
    
    MD5_Init(&ctx);
    MD5_Update(&ctx, kcat, 80);
    MD5_Final(out, &ctx);
    
    return out;
}
