//
//  smbBuffer.m
//  test
//
//  Created by trekvn on 4/13/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import "config.h"
#import "smbBuffer.h"

#ifdef HAVE_ALLOCA_H
#   import <alloca.h>
#endif

@implementation smbBuffer
#pragma mark - smbBufferInit
void smb_buffer_init(smb_buffer *buf, void *data, size_t size) {
    assert(buf != NULL);
    
    buf->data = data;
    buf->size = size;
}

#pragma mark - smbBufferAlloc
int smb_buffer_alloc(smb_buffer *buf, size_t size) {
    assert(buf != NULL);
    
    buf->data = malloc(size);
    if (buf->data) {
        buf->size = size;
        return 1;
    } else
        return 0;
}

#pragma mark - smbBufferFree
void smb_buffer_free(smb_buffer *buf) {
    if (buf == NULL || buf->data == NULL)
        return;
    
    free(buf->data);
    smb_buffer_init(buf, NULL, 0);
}
@end
