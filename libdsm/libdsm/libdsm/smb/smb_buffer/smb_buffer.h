//
//  smb_buffer.h
//  test
//
//  Created by trekvn on 4/7/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef smb_buffer_h
#define smb_buffer_h

#include <stdio.h>
#include <stdlib.h>

/**
 * @struct smb_buffer
 * @brief Hold a pointer and the size of its data
 */
typedef struct
{
    void      *data;  /// Data pointed
    size_t    size;   /// Size in byte of the pointed
} smb_buffer;

/**
 * @brief Initialize a buffer structure. It'll contain nothing
 *
 * @param buf Pointer to a buffer to initialize
 */
void    smb_buffer_init(smb_buffer *buf, void *data, size_t size);

/**
 * @brief Allocate a size long memory area and place it in the buffer structure
 */
int     smb_buffer_alloc(smb_buffer *buf, size_t size);

/**
 * @brief Free the data of this buffer if necessary
 *
 * @param buf Pointer to a buffer to free
 */
void    smb_buffer_free(smb_buffer *buf);

#endif /* smb_buffer_h */
