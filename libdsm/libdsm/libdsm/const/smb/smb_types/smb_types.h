//
//  smb_types.h
//  test
//
//  Created by trekvn on 4/7/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef smb_types_h
#define smb_types_h

#include <stddef.h>
#include <stdint.h>

#include <stddef.h>
#include <stdbool.h>

#include "libtasn1.h"

#if !defined _WIN32
# include <netinet/ip.h>
#else
# include <winsock2.h>
#endif

#include "smb_buffer.h"
#include "smb_packets.h"


#define _FILE_OFFSET_BITS 64
#if defined(__ANDROID__)
# undef  off_t
# define off_t off64_t
#endif
/*!smb_tid
 * The id of a connection to a share within a session.
 */
typedef uint16_t    smb_tid;

/*!smb_fid
 * The id of a file within a share within a session.
 */
typedef uint16_t    smb_fid;

// Concatenation of the two above, representing a file inside of a session
// First 4 bytes are the TreeID (smb_tid), last 4 are the File ID (FUID)
// A map between smb_fd and smb_file is maintained inside each session

/*!smb_fd
 * SMB File descriptor, represents a file within a session.
 */
typedef uint32_t    smb_fd;

// An structure to store user credentials;
// login:password@domain (also DOMAIN\login)
typedef struct
{
    char     *domain;
    char     *login;
    char     *password;
}           smb_creds;

/**
 * @brief An opaque data structure to represent a SMB Session.
 */
typedef struct smb_session smb_session;

/*!smb_share_list
 * An opaque object representing the list of share of a SMB file server.
 */
typedef char  **smb_share_list;

/**
 * @brief An opaque data structure to represent file
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

/**
 * @internal
 * @struct smb_file
 * @brief An opaque data structure to represent file
 */
struct smb_file
{
    smb_file            *next;          // Next file in this share
    char                *name;
    smb_fid             fid;
    smb_tid             tid;
    size_t              name_len;
    uint64_t            created;
    uint64_t            accessed;
    uint64_t            written;
    uint64_t            changed;
    uint64_t            alloc_size;
    uint64_t            size;
    uint32_t            attr;
    off_t               offset;          // Current position pointer
    int                 is_dir;         // 0 -> file, 1 -> directory
};

typedef struct smb_share smb_share;
struct smb_share
{
    smb_share           *next;          // Next share in this session
    smb_file            *files;         // List of all open files for this share
    smb_tid             tid;
    uint16_t            opts;           // Optionnal support opts
    uint16_t            rights;         // Maximum rights field
    uint16_t            guest_rights;
};

typedef struct smb_transport smb_transport;
struct smb_transport
{
    void              *session;
    void              *(*new)(size_t buf_size);
    int               (*connect)(uint32_t ip, void *s, const char *name);
    void              (*destroy)(void *s);
    void              (*pkt_init)(void *s);
    int               (*pkt_append)(void *s, void *data, size_t size);
    int               (*send)(void *s);
    ssize_t           (*recv)(void *s, void **data);
};

typedef struct smb_srv_info smb_srv_info;
struct smb_srv_info
{
    char                name[16];       // The server name
    uint16_t            dialect;        // The selected dialect
    uint16_t            security_mode;  // Security mode
    uint16_t            uid;            // uid attributed by the server.
    uint32_t            session_key;    // The session key sent by the server on protocol negotiate
    uint32_t            caps;           // Server caps replyed during negotiate
    uint64_t            challenge;      // For challenge response security
    uint64_t            ts;             // It seems Win7 requires it :-/
};

/**
 * @brief An opaque data structure to represent a SMB Session.
 */
struct smb_session
{
    bool                logged;
    bool                guest;            // Are we logged as guest ?
    
    // Informations about the smb server we are connected to.
    smb_srv_info        srv;
    
    
    ASN1_TYPE           spnego_asn1;
    smb_buffer          xsec_target;
    
    smb_creds           creds;
    smb_transport       transport;
    
    smb_share           *shares;          // shares->files | Map fd <-> smb_file
    uint32_t            nt_status;
};

typedef struct smb_message smb_message;
struct smb_message
{
    size_t          payload_size; // Size of the allocated payload
    size_t          cursor;       // Write cursor in the payload
    smb_packet      *packet;      // Yummy yummy, Fruity fruity !
};

#endif /* smb_types_h */
