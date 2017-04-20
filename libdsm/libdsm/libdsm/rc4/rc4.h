//
//  rc4.h
//  test
//
//  Created by trekvn on 4/13/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>
struct rc4_state {
    uint8_t  perm[256];
    uint8_t  index1;
    uint8_t  index2;
};

@interface rc4 : NSObject

#pragma mark - rc4Init
extern void rc4_init(struct rc4_state *state, const uint8_t *key, int keylen);

#pragma mark - rc4Crypt
/*!Encrypt some data using the supplied RC4 state buffer. The input and output buffers may be the same buffer.
 * Since RC4 is a stream cypher, this function is used for both encryption and decryption.
 */
extern void rc4_crypt(struct rc4_state *state,
                      const uint8_t *inbuf, uint8_t *outbuf, int buflen);
@end
#endif
