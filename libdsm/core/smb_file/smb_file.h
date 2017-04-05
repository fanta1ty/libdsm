//
//  smb_file.h
//  libdsm
//
//  Created by trekvn on 4/5/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef smb_file_h
#define smb_file_h

#include <stdio.h>
#include "smb_session.h"

/*!Open a file on a share.
 * Use this function to obtain an smb_fd, necesary for file operations
 *\param s The session object
 *\param tid The tid of the share the file is in, obtained via smb_tree_connect()
 *\param path The path of the file to open
 *\param mod The access modes requested (example: #SMB_MOD_RO)
 *\param fd The pointer to the smb file description that can be used for
 * further file operations
 *\returns 0 on success or a DSM error code in case of error
 */
int smb_fopen(smb_session *s, smb_tid tid, const char *path,
              uint32_t mod, smb_fd *fd);

/*!Close an open file
 * The smb_fd is invalidated and MUST not be use it anymore. You can
 * give it the 0 value.
 *\param s The session object
 *\param fd The SMB file descriptor
 */
void smb_fclose(smb_session *s, smb_fd fd);

/*!Read from an open file
 * The semantics is basically the same that the unix read() one.
 * At most 'buf_size' bytes are read from the current seek offset and copied into
 * the memory pointed by 'buf' from the open file represented by the smb file
 * descriptor 'fd'.
 *\param s The session object
 *\param fd [description]
 *\param buf can be NULL in order to skip buf_size bytes
 *\param buf_size [description]
 *\returns The number of bytes read or -1 in case of error.
 */
ssize_t smb_fread(smb_session *s, smb_fd fd, void *buf, size_t buf_size);

/*!Write to an open file
 * At most 'buf_size' bytes from memory pointed by 'buf' are written
 * to the current seek offset of the open file represented by the smb file
 * descriptor 'fd'.
 *\param s The session object
 *\param fd [description]
 *\param buf [description]
 *\param buf_size [description]
 *\returns The number of bytes written or -1 in case of error.
 */
ssize_t smb_fwrite(smb_session *s, smb_fd fd, void *buf, size_t buf_size);

/*!Sets/Moves/Get the read/write pointer for a given file
 * The behavior of this function is the same as the Unix fseek()
 * function, except the SEEK_END argument isn't supported.
 * This functions adjust the read/write pointer depending on the value of
 * offset and whence.
 * - If whence == #SMB_SEEK_SET, the read pointer is set at 'offset'
 * - If whence == #SMB_SEEK_CUR, the read pointer is adjusted by 'offset'
 *\param s The session object
 *\param fd The file descriptors for which the read pointer is to be adjusted
 *\param offset Set/Adjust quantity
 *\param whence Which action to perform. Supported operations are
 * #SMB_SEEK_SET and #SMB_SEEK_CUR
 *\returns The current read pointer position or -1 on error
 */
ssize_t smb_fseek(smb_session *s, smb_fd fd, off_t offset, int whence);

/*!Remove a file on a share.
 * Use this function to delete a file
 *\param s The session object
 *\param tid The tid of the share the file is in, obtained via smb_tree_connect()
 *\param path The path of the file to delete
 *\returns 0 if delete OK or "NT" error code
 */
int  smb_file_rm(smb_session *s, smb_tid tid, const char *path);

/*!Move/rename a file/directory on a share.
 * Use this function to move and/or rename a file/directory
 *\param s The session object
 *\param tid The tid of the share the file is in, obtained via smb_tree_connect()
 *\param old_path The current path of the file/directory to move/rename
 *\param new_path The new path of the file/directory
 *\returns 0 if move OK or -1 in case of error
 */
int smb_file_mv(smb_session *s, smb_tid tid, const char *old_path, const char *new_path);

#endif /* smb_file_h */
