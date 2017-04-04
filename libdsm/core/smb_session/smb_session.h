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
#include "smb_defs.h"
#include "smb_types.h"

#define SMB_CREDS_MAXLEN            128

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

#endif /* smb_session_h */
