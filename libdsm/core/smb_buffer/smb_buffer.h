//
//  smb_buffer.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef smb_buffer_h
#define smb_buffer_h

#include <stdio.h>
#include <stdlib.h>

/*!smb_buffer
 * Hold a pointer and the size of its data
 */
typedef struct {
    void      *data;    //--> Data pointed
    size_t    size;     //--> Size in byte of the pointed
} smb_buffer;

/*!Initialize a buffer structure. 
 * It'll contain nothing
 *\param buf Pointer to a buffer to initialize
 */
void smb_buffer_init(smb_buffer *buf, void *data, size_t size);

/*!Allocate a size long memory area and place it in the buffer structure
 */
int smb_buffer_alloc(smb_buffer *buf, size_t size);

/*!Free the data of this buffer if necessary
 *\param buf Pointer to a buffer to free
 */
void smb_buffer_free(smb_buffer *buf);
#endif /* smb_buffer_h */
