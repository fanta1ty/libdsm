//
//  smb_buffer.c
//  test
//
//  Created by trekvn on 4/7/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#include "config.h"

#include <assert.h>
#include <stdlib.h>

#ifdef HAVE_ALLOCA_H
# include <alloca.h>
#endif

#include "smb_buffer.h"

void    smb_buffer_init(smb_buffer *buf, void *data, size_t size)
{
    assert(buf != NULL);
    
    buf->data = data;
    buf->size = size;
}

int     smb_buffer_alloc(smb_buffer *buf, size_t size)
{
    assert(buf != NULL);
    
    buf->data = malloc(size);
    if (buf->data) {
        buf->size = size;
        return 1;
    } else
        return 0;
}

void    smb_buffer_free(smb_buffer *buf)
{
    if (buf == NULL || buf->data == NULL)
        return;
    
    free(buf->data);
    smb_buffer_init(buf, NULL, 0);
}
