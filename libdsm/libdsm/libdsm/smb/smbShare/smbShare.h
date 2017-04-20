//
//  smbShare.h
//  test
//
//  Created by trekvn on 4/13/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import "smbHeader.h"

@interface smbShare : NSObject

#pragma mark - smbShareGetList
/*! List the existing share of this sessions's machine
 * This function makes a RPC to the machine this session is currentl authenticated to and list all the existing shares of this machines. The share starting with a $ are supposed to be system/hidden share.
 *\param[in] s The session object
 *\param[out] list A pointer to an opaque share_list object.
 *\param[out] p_count to the number of elements in the list
 *\returns 0 on success or a DSM error code in case of error
 */
int smb_share_get_list(smb_session *s, smb_share_list *list, size_t *p_count);

#pragma mark - smbShareListCount
/*!Get the number of share in the list
 *\@param list An opaque share list returned by smb_share_list()
 *\returns The number of share in the opaque share_list object
 */
size_t smb_share_list_count(smb_share_list list);

#pragma mark - smbShareListAt
/*!Get the name of the share in the list at the given index
 *\param list An opaque share list object
 *\param index The index of the returned item in the list
 *\returns The string has been decoded from UTF16 to you local encoding
 */
const char *smb_share_list_at(smb_share_list list, size_t index);

#pragma mark - smbShareListDestroy
/*!Destroy an opaque share list object
 *\param list The list to destroy. The object is not usable anymore afterward, you can set it to 'NULL'
 */
void smb_share_list_destroy(smb_share_list list);

#pragma mark - smbTreeConnect
/*!Connects to a SMB share
 * Before being able to list/read files on a SMB file server, you have to be connected to the share containing the files you want to read or the directories you want to list
 *\param s The session object
 *\param name The share name @see smb_share_list
 *\param tid The pointer to an opaque open share
 *\returns 0 on success or a DSM error code in case of error
 */
int smb_tree_connect(smb_session *s, const char *name, smb_tid *tid);

#pragma mark - smbTreeDisconnect
/*!Disconnect from a share
 * UNIMPLEMENTED
 *\returns 0 on success or a DSM error code in case of error
 */
int smb_tree_disconnect(smb_session *s, smb_tid tid);
@end
#endif
