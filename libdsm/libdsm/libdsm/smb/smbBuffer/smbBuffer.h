//
//  smbBuffer.h
//  test
//
//  Created by trekvn on 4/13/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>

/*!Hold a pointer and the size of its data
 */
typedef struct
{
    void      *data;  /// Data pointed
    size_t    size;   /// Size in byte of the pointed
} smb_buffer;


@interface smbBuffer : NSObject
#pragma mark - smbBufferInit
/*!Initialize a buffer structure. It'll contain nothing
 *\param buf Pointer to a buffer to initialize
 */
void smb_buffer_init(smb_buffer *buf, void *data, size_t size);

#pragma mark - smbBufferAlloc
/*!Allocate a size long memory area and place it in the buffer structure
 */
int smb_buffer_alloc(smb_buffer *buf, size_t size);

#pragma mark - smbBufferFree
/*!Free the data of this buffer if necessary
 *\param buf Pointer to a buffer to free
 */
void smb_buffer_free(smb_buffer *buf);
@end
#endif
