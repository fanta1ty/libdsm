//
//  smbDir.h
//  test
//
//  Created by trekvn on 4/13/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>

#import "smbHeader.h"
@interface smbDir : NSObject

#pragma mark - smbDirectoryRM
/*!remove a directory on a share.
 * Use this function to delete an empty directory
 *\param s The session object
 *\param tid The tid of the share the file is in, obtained via smb_tree_connect()
 * @param path The path of the file to delete
 * @returns 0 on success or a DSM error code in case of error
 */
int smb_directory_rm(smb_session *s, smb_tid tid, const char *path);

#pragma mark - smbDirectoryCreate
/*!Create a directory on a share.
 * Use this function to create a directory
 *\param s The session object
 *\param tid The tid of the share the file is in, obtained via smb_tree_connect()
 *\param path The path of the directory to create
 *\returns 0 on success or a DSM error code in case of error
 */
int smb_directory_create(smb_session *s, smb_tid tid, const char *path);

@end
#endif
