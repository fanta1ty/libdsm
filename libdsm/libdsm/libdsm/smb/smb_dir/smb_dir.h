//
//  smb_dir.h
//  test
//
//  Created by trekvn on 4/10/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef smb_dir_h
#define smb_dir_h

#include <stdio.h>
#include "smb_session.h"


/**
 * @brief remove a directory on a share.
 * @details Use this function to delete an empty directory
 *
 * @param s The session object
 * @param tid The tid of the share the file is in, obtained via smb_tree_connect()
 * @param path The path of the file to delete
 * @return 0 on success or a DSM error code in case of error
 */
int smb_directory_rm(smb_session *s, smb_tid tid, const char *path);

/**
 * @brief create a directory on a share.
 * @details Use this function to create a directory
 *
 * @param s The session object
 * @param tid The tid of the share the file is in, obtained via smb_tree_connect()
 * @param path The path of the directory to create
 * @return 0 on success or a DSM error code in case of error
 */
int smb_directory_create(smb_session *s, smb_tid tid, const char *path);
#endif /* smb_dir_h */
