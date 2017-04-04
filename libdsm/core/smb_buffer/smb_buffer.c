//
//  smb_buffer.c
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#include "smb_buffer.h"
#include <assert.h>
#include <stdlib.h>
#include <alloca.h>

#ifdef HAVE_ALLOCA_H
#   include <alloca.h>
#endif

void smb_buffer_init(smb_buffer *buf, void *data, size_t size) {
    assert(buf != NULL);
    
    buf->data = data;
    buf->size = size;
}

int smb_buffer_alloc(smb_buffer *buf, size_t size) {
    assert(buf != NULL);
    
    buf->data = malloc(size);
    if (buf->data) {
        buf->size = size;
        return 1;
    } else
        return 0;
}

void smb_buffer_free(smb_buffer *buf) {
    if (buf == NULL || buf->data == NULL)
        return;
    
    free(buf->data);
    smb_buffer_init(buf, NULL, 0);
}
