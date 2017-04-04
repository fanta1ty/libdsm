//
//  smb_session.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef smb_session_h
#define smb_session_h

#include <stdio.h>
#include <stddef.h>

#include "smb_defs.h"
#include "smb_types.h"
#include "smb_message.h"

#define SMB_CREDS_MAXLEN                128
#define SMB_FD_TID(fd)                  ((smb_tid)(fd >> 16))
#define SMB_FD_FID(fd)                  ((smb_fid)(fd & 0x0000ffff))
#define SMB_FD(tid, fid)                ((((smb_fd)tid) << 16) | (((smb_fd) fid)))
#define SMB_SESSION_MAX_BUFFER          (0xffff)
/*!Allocates a new Session object
 * To be able to perform actions on shares and file, you'll need to
 * call smb_session_connect, then authenticate with smb_authenticate.
 *\returns a new session object.
 */
smb_session *smb_session_new();

/*!Close a session and deallocate its ressources
 * After destroying a session, all the smb_tid, smb_fid and smb_fd
 * associated with this session becomes invalid.
 *\param s The session to destroy
 */
void smb_session_destroy(smb_session *s);

/*!Set the credentials for this session.
 * Any of the params except s can be NULL.
 *\param s The session object.
 *\param domain Domain to authenticate on. Often it's the same as netbios host.
 *\param login The user to login as.
 *\param password the user's password.
 */
void smb_session_set_creds(smb_session *s, const char *domain,
                           const char *login, const char *password);

/*!Establish a connection and negotiate a session protocol with a remote
 * host
 * You have to provide both the ip and the name. This is a constraint
 * of Netbios, which requires you to know its name before he accepts to speak
 * with you.
 *\param s A session object.
 *\param hostname The ASCII netbios name, the name type will be coerced to <20>
 * since libdsm is about reading files
 *\param ip The ip of the machine to connect to (in network byte order)
 *\param transport The type of transport used, it could be SMB_TRANSPORT_TCP
 * or SMB_TRANSPORT_NBT (Netbios over TCP, ie legacy)
 *\returns 0 on success or a DSM error code in case of error
 */
int smb_session_connect(smb_session *s, const char *hostname,
                        uint32_t ip, int transport);

/*!Authenticate on the remote host with the provided credentials
 * Can be called if session state is SMB_STATE_DIALECT_OK.
 * If successfull, session state transition to SMB_STATE_SESSION_OK
 * Provides the credentials with smb_session_set_creds.
 *\param s The session object.
 *\returns 0 on success or a DSM error code in case of error. Success doesn't
 * mean you are logged in with the user you requested. If guest are activated
 * on the remote host, when login fails, you are logged in as 'Guest'. Failure
 * might also indicate you didn't supplied all the credentials
 */
int smb_session_login(smb_session *s);

/*!Am i logged in as Guest ?
 *\param s The session object
 *\returns  1  -> Logged in as guest
 *          0  -> Logged in as regular user
 *          -1 -> Error (not logged in, invalid session, etc.)
 */
int smb_session_is_guest(smb_session *s);

/*!Returns the server name with the <XX> type
 *\param s The session object
 *\returns The server name or NULL. The memory is owned by the session object.
 */
const char *smb_session_server_name(smb_session *s);


/*!Check if a feature is supported/has been negociated with the server
 *\param s The session object
 *\param what Which features to check ? @see smb_session_supports_what
 *\returns 0 if the feature is not supported, something else otherwise
 */
int smb_session_supports(smb_session *s, int what);

/*!Get the last NT_STATUS
 * Valid only if a smb_ function returned the DSM_ERROR_NT error.
 *\param s The session object
 */
uint32_t smb_session_get_nt_status(smb_session *s);

bool smb_session_check_nt_status(smb_session *s, smb_message *msg);
#endif /* smb_session_h */
