//
//  smb_types.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef smb_types_h
#define smb_types_h

#include <stddef.h>
#include <stdint.h>
#define _FILE_OFFSET_BITS           64

#if defined(__ANDROID__)
#   undef off_t
#   define off_t off64_t
#endif

/*!smb_tid
 * The id of a connection to a share within a session.
 */
typedef uint16_t smb_tid;

/*!smb_fid
 * The id of a file within a share within a session.
 */
typedef uint16_t smb_fid;

// Concatenation of the two above, representing a file inside of a session
// First 4 bytes are the TreeID (smb_tid), last 4 are the File ID (FUID)
// A map between smb_fd and smb_file is maintained inside each session

/*!smb_fd
 * SMB File descriptor, represents a file within a session.
 */
typedef uint32_t smb_fd;

// An structure to store user credentials;
// login:password@domain (also DOMAIN\login)
typedef struct {
    char     *domain;
    char     *login;
    char     *password;
}smb_creds;

/*!An opaque data structure to represent a SMB Session.
 */
typedef struct smb_session smb_session;

/*!smb_share_list
 * An opaque object representing the list of share of a SMB file server.
 */
typedef char **smb_share_list;

/*!An opaque data structure to represent file
 */
typedef struct smb_file smb_file;

/*!smb_stat_list
 * An opaque structure containing a list of file status
 */
typedef smb_file *smb_stat_list;

/*!smb_stat
 * An opaque structure containing info about a file
 */
typedef smb_file *smb_stat;

#endif /* smb_types_h */
