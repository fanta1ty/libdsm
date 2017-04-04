//
//  smb_defs.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef smb_defs_h
#define smb_defs_h

#define SMB_DEFAULT_BUFSIZE             (8192)

enum {
    SMB_TRANSPORT_TCP = 1,                  //--> SMB with Direct-TCP connection (OSX supports only this)
    SMB_TRANSPORT_NBT = 2                   //--> SMB with Netbios over TCP (older mechanism)
};

/*!SMB Session states
 */
enum {
    SMB_STATE_ERROR = -1,                   //--> Error state, there was an error somewhere
    SMB_STATE_NEW = 0,                      //--> The SMB session has just been created
    SMB_STATE_NETBIOS_OK = 1,               //--> A Netbios session has been successfully established.
    SMB_STATE_DIALECT_OK = 2,               //--> Dialect was successfully negotiated
    SMB_STATE_SESSION_OK = 3                //--> Session Authentication was successfull,
                                            //you can become nasty
};

/*!smb_fseek operations
 */
enum {
    SMB_SEEK_SET = 0,                       //--> Set the read pointer at the given position
    SMB_SEEK_CUR = 1                        //--> Adjusts the read pointer relatively to the actual position
};

enum smb_session_supports_what {
    SMB_SESSION_XSEC = 0,
};

/*!File access rights (used when smb_open() files)
 */
#define SMB_MOD_READ            (1 << 0)    //--> Flag for smb_file_open. Request right for reading
#define SMB_MOD_WRITE           (1 << 1)    //--> Flag for smb_file_open. Request right for writing
#define SMB_MOD_APPEND          (1 << 2)    //--> Flag for smb_file_open. Request right for appending
#define SMB_MOD_READ_EXT        (1 << 3)    //--> Flag for smb_file_open. Request right for extended
                                            //read (?)
#define SMB_MOD_WRITE_EXT       (1 << 4)    //--> Flag for smb_file_open. Request right for extended
                                            //write (?)
#define SMB_MOD_EXEC            (1 << 5)    //--> Flag for smb_file_open. Request right for execution (?)
#define SMB_MOD_RMCHILD         (1 << 6)    //--> Flag for smb_file_open. Request right for child
                                            //removal (?)
#define SMB_MOD_READ_ATTR       (1 << 7)    //--> Flag for smb_file_open. Request right for reading
                                            //file attributes
#define SMB_MOD_WRITE_ATTR      (1 << 8)    //--> Flag for smb_file_open. Request right for writing
                                            //file attributes
#define SMB_MOD_RM              (1 << 16)   //--> Flag for smb_file_open. Request right for removing
                                            //file
#define SMB_MOD_READ_CTL        (1 << 17)   //--> Flag for smb_file_open. Request right for reading ACL
#define SMB_MOD_WRITE_DAC       (1 << 18)   //--> Flag for smb_file_open. Request right for writing ACL
#define SMB_MOD_CHOWN           (1 << 19)   //--> Flag for smb_file_open. Request right for changing owner
#define SMB_MOD_SYNC            (1 << 20)   //--> Flag for smb_file_open. (??)
#define SMB_MOD_SYS             (1 << 24)   //--> Flag for smb_file_open. (??)
#define SMB_MOD_MAX_ALLOWED     (1 << 25)   //--> Flag for smb_file_open. (??)
#define SMB_MOD_GENERIC_ALL     (1 << 28)   //--> Flag for smb_file_open. Request all generic rights (??)
#define SMB_MOD_GENERIC_EXEC    (1 << 29)   //--> Flag for smb_file_open. Request generic exec right (??)
#define SMB_MOD_GENERIC_READ    (1 << 30)   //--> Flag for smb_file_open. Request generic read right (??)
#define SMB_MOD_GENERIC_WRITE   (1 << 31)   //--> Flag for smb_file_open. Request generic write right (??)

/*!Flag for smb_file_open. Default R/W mode
 * @details A few flags OR'ed
 */
#define SMB_MOD_RW              (SMB_MOD_READ | SMB_MOD_WRITE | SMB_MOD_APPEND | \
                                 SMB_MOD_READ_EXT | SMB_MOD_WRITE_EXT |\
                                 SMB_MOD_READ_ATTR | SMB_MOD_WRITE_ATTR | SMB_MOD_READ_CTL )

/*!Flag for smb_file_open. Default R/O mode
 * @details A few flags OR'ed
 */
#define SMB_MOD_RO              (SMB_MOD_READ | SMB_MOD_READ_EXT | SMB_MOD_READ_ATTR | \
                                 SMB_MOD_READ_CTL )

/*!NTSTATUS & internal return codes
 */
#define NT_STATUS_SUCCESS                   0x00000000
#define NT_STATUS_INVALID_SMB               0x00010002
#define NT_STATUS_SMB_BAD_TID               0x00050002
#define NT_STATUS_SMB_BAD_UID               0x005b0002
#define NT_STATUS_NOT_IMPLEMENTED           0xc0000002
#define NT_STATUS_INVALID_DEVICE_REQUEST    0xc0000010
#define NT_STATUS_NO_SUCH_DEVICE            0xc000000e
#define NT_STATUS_NO_SUCH_FILE              0xc000000f
#define NT_STATUS_MORE_PROCESSING_REQUIRED  0xc0000016
#define NT_STATUS_INVALID_LOCK_SEQUENCE     0xc000001e
#define NT_STATUS_INVALID_VIEW_SIZE         0xc000001f
#define NT_STATUS_ALREADY_COMMITTED         0xc0000021
#define NT_STATUS_ACCESS_DENIED             0xc0000022
#define NT_STATUS_OBJECT_NAME_NOT_FOUND     0xc0000034
#define NT_STATUS_OBJECT_NAME_COLLISION     0xc0000035
#define NT_STATUS_OBJECT_PATH_INVALID       0xc0000039
#define NT_STATUS_OBJECT_PATH_NOT_FOUND     0xc000003a
#define NT_STATUS_OBJECT_PATH_SYNTAX_BAD    0xc000003b
#define NT_STATUS_PORT_CONNECTION_REFUSED   0xc0000041
#define NT_STATUS_THREAD_IS_TERMINATING     0xc000004b
#define NT_STATUS_DELETE_PENDING            0xc0000056
#define NT_STATUS_PRIVILEGE_NOT_HELD        0xc0000061
#define NT_STATUS_LOGON_FAILURE             0xc000006D
#define NT_STATUS_DFS_EXIT_PATH_FOUND       0xc000009b
#define NT_STATUS_MEDIA_WRITE_PROTECTED     0xc00000a2
#define NT_STATUS_ILLEGAL_FUNCTION          0xc00000af
#define NT_STATUS_FILE_IS_A_DIRECTORY       0xc00000ba
#define NT_STATUS_FILE_RENAMED              0xc00000d5
#define NT_STATUS_REDIRECTOR_NOT_STARTED    0xc00000fb
#define NT_STATUS_DIRECTORY_NOT_EMPTY       0xc0000101
#define NT_STATUS_PROCESS_IS_TERMINATING    0xc000010a
#define NT_STATUS_TOO_MANY_OPENED_FILES     0xc000011f
#define NT_STATUS_CANNOT_DELETE             0xc0000121
#define NT_STATUS_FILE_DELETED              0xc0000123
#define NT_STATUS_INSUFF_SERVER_RESOURCES   0xc0000205

#define DSM_SUCCESS                         (0)
#define DSM_ERROR_GENERIC                   (-1)
#define DSM_ERROR_NT                        (-2) /* see smb_session_get_nt_status */
#define DSM_ERROR_NETWORK                   (-3)
#define DSM_ERROR_CHARSET                   (-4)


/*!Our own identity
 */
#define SMB_OS                              "Unix"
#define SMB_LANMAN                          "liBDSM"

/*!A few magic stuffs (SMB magic // our dialect support)
 */
#define SMB_MAGIC               { 0xff, 0x53, 0x4d, 0x42 }  // aka "\xffSMB"
#define SMB_DIALECTS {                                                      \
    "\2Samba",                                                              \
    "\2NT LM 0.12",                                                         \
    NULL                                                                    \
}

/*!Dialect values must match position on SMB_DIALECTS array
 */
#define SMB_DIALECT_SAMBA                           0
#define SMB_DIALECT_NTLM                            1

/*!SMB Operations/Commands
 */
#define SMB_CMD_CLOSE                               0x04
#define SMD_CMD_TRANS                               0x25
#define SMB_CMD_TRANS2                              0x32
#define SMB_CMD_TREE_DISCONNECT                     0x71
#define SMB_CMD_NEGOTIATE                           0x72
#define SMB_CMD_SETUP                               0x73    // Session Setup AndX
#define SMB_CMD_TREE_CONNECT                        0x75    // Tree Connect AndX
#define SMB_CMD_ECHO                                0x2b
#define SMB_CMD_READ                                0x2e    // Read AndX
#define SMB_CMD_WRITE                               0x2f    // Write AndX
#define SMB_CMD_CREATE                              0xa2    // NT Create AndX
#define SMB_CMD_MKDIR                               0x00    // Depecated
#define SMB_CMD_RMDIR                               0x01
#define SMB_CMD_RMFILE                              0x06
#define SMB_CMD_MOVE                                0x07    // Move or rename

/*!SMB FLAGS2 values
 */
#define SMB_FLAGS2_SHORT_NAMES                      0x0000
#define SMB_FLAGS2_LONG_NAMES                       0x0001

/*!SMB TRANS2 SubCommands
 */
#define SMB_TR2_FIND_FIRST                          0x0001
#define SMB_TR2_FIND_NEXT                           0x0002
#define SMB_TR2_QUERY_PATH                          0x0005
#define SMB_TR2_CREATE_DIRECTORY                    0x000d

/*!SMB TRANS2 FIND interest values
 */
#define SMB_FIND2_INTEREST_DIRECTORY_INFO           0x0101
#define SMB_FIND2_INTEREST_FULL_DIRECTORY_INFO      0x0102
#define SMB_FIND2_INTEREST_NAMES_INFO               0x0103
#define SMB_FIND2_INTEREST_BOTH_DIRECTORY_INFO      0x0104

/*!SMB TRANS2 QUERY (FILE & PATH) interest values
 */
#define SMB_FIND2_QUERY_FILE_BASIC_INFO             0x0101
#define SMB_FIND2_QUERY_FILE_STANDARD_INFO          0x0102
#define SMB_FIND2_QUERY_FILE_EA_INFO                0x0103
#define SMB_FIND2_QUERY_FILE_NAME_INFO              0x0104
#define SMB_FIND2_QUERY_FILE_ALL_INFO               0x0107
#define SMB_FIND2_QUERY_FILE_ALT_NAME_INFO          0x0108
#define SMB_FIND2_QUERY_FILE_STREAM_INFO            0x0109
#define SMB_FIND2_QUERY_FILE_COMPRESSION_INFO       0x010B

/*!SMB CMD CREATE Impersonation level values
 */
#define SMB_IMPERSONATION_SEC_ANONYMOUS             0
#define SMB_IMPERSONATION_SEC_IDENTIFY              1
#define SMB_IMPERSONATION_SEC_IMPERSONATE           2

/*!Flags definitions
 * Many aren't use in libdsm but are here for possible later use
 */
// Protocol negotiation flags (flags field in spec)
#define SMB_FLAG_RESPONSE                           (1 << 7)
#define SMB_FLAG_NOTIFY                             (1 << 6)
#define SMB_FLAG_OPLOCK                             (1 << 5)
#define SMB_FLAG_CANONIC                            (1 << 4)
#define SMB_FLAG_CASELESS                           (1 << 3)
#define SMB_FLAG_BUFFER_POSTED                      (1 << 1)
#define SMB_FLAG_LOCK_AND_READ                      (1 << 0)

// More Protocol negotiation flags (flags2 field in spec)
#define SMB_FLAG_UNICODE                            (1 << (15 + 8))
#define SMB_FLAG_NT_ERRORS                          (1 << (14 + 8))
#define SMB_FLAG_EXECUTE_ONLY                       (1 << (13 + 8))
#define SMB_FLAG_DFS                                (1 << (12 + 8))
#define SMB_FLAG_XSEC                               (1 << (11 + 8))
#define SMB_FLAG_REPARSE_PATH                       (1 << (10 + 8))
#define SMB_FLAG_LONG_NAMES                         (1 << (6 + 8))
#define SMB_FLAG_SIGN_REQUIRED                      (1 << (4 + 8))
#define SMB_FLAG_COMPRESSED                         (1 << (3 + 8))
#define SMB_FLAG_SIGN_SUPPORT                       (1 << (2 + 8))
#define SMB_FLAG_EXT_ATTR                           (1 << (1 + 8))
#define SMB_FLAG_LONG_NAMES_OK                      (1 << (0 + 8))

// Negotiated server capabilities
#define SMB_CAPS_RAW                                (1 << 0)
#define SMB_CAPS_MPX                                (1 << 1)
#define SMB_CAPS_UNICODE                            (1 << 2)
#define SMB_CAPS_LARGE                              (1 << 3)
#define SMB_CAPS_NTSMB                              (1 << 4)
#define SMB_CAPS_RPC                                (1 << 5)
#define SMB_CAPS_NTFIND                             (1 << 9)
#define SMB_CAPS_XSEC                               (1 << 31)

// File creation/open flags
#define SMB_CREATE_OPLOCK                           (1 << 1)
#define SMB_CREATE_BATCH_OPLOCK                     (1 << 2)
#define SMB_CREATE_MKDIR                            (1 << 3)
#define SMB_CREATE_EXT_RESP                         (1 << 4)
#define SMB_CREATE_DEFAULTS                         (0)

// File attributes
#define SMB_ATTR_NORMAL                             0
#define SMB_ATTR_RO                                 (1 << 0)
#define SMB_ATTR_HIDDEN                             (1 << 1)
#define SMB_ATTR_SYS                                (1 << 2)
#define SMB_ATTR_VOLID                              (1 << 3)  // Volume ID
#define SMB_ATTR_DIR                                (1 << 4)
#define SMB_ATTR_ARCHIVE                            (1 << 5)  // Modified since last archive (!?)

// Share access flags
#define SMB_SHARE_READ                              (1 << 0)
#define SMB_SHARE_WRITE                             (1 << 1)
#define SMB_SHARE_DELETE                            (1 << 2)

// Create disposition flags
#define SMB_DISPOSITION_FILE_SUPERSEDE              0
#define SMB_DISPOSITION_FILE_OPEN                   (1 << 0)
#define SMB_DISPOSITION_FILE_CREATE                 (1 << 1)
#define SMB_DISPOSITION_FILE_OPEN_IF                (1 << 2)
#define SMB_DISPOSITION_FILE_OVERWRITE              (1 << 3)
#define SMB_DISPOSITION_FILE_OVERWRITE_IF           (1 << 4)

// Create options flags
#define SMB_CREATEOPT_DIRECTORY_FILE                (1 << 0)
#define SMB_CREATEOPT_WRITE_THROUGH                 (1 << 1)
#define SMB_CREATEOPT_SEQUENTIAL_ONLY               (1 << 2)
#define SMB_CREATEOPT_NO_INTERMEDIATE_BUFFERING     (1 << 3)
#define SMB_CREATEOPT_SYNCHRONOUS_IO_ALERT          (1 << 3)
#define SMB_CREATEOPT_SYNCHRONOUS_IO_NONALERTIF     (1 << 4)
#define SMB_CREATEOPT_NON_DIRECTORY_FILE            (1 << 5)
#define SMB_CREATEOPT_CREATE_TREE_CONNECTION        (1 << 6)
#define SMB_CREATEOPT_COMPLETE_IF_OPLOCKED          (1 << 7)
#define SMB_CREATEOPT_NO_EA_KNOWLEDGE               (1 << 8)
#define SMB_CREATEOPT_OPEN_FOR_RECOVERY             (1 << 9)
#define SMB_CREATEOPT_RANDOM_ACCESS                 (1 << 10)
#define SMB_CREATEOPT_DELETE_ON_CLOSE               (1 << 11)
#define SMB_CREATEOPT_OPEN_BY_FILE_ID               (1 << 12)
#define SMB_CREATEOPT_OPEN_FOR_BACKUP_INTENT        (1 << 13)
#define SMB_CREATEOPT_NO_COMPRESSION                (1 << 14)
#define SMB_CREATEOPT_RESERVE_OPFILTER              (1 << 15)
#define SMB_CREATEOPT_OPEN_NO_RECALL                (1 << 16)
#define SMB_CREATEOPT_OPEN_FOR_FREE_SPACE_QUERY     (1 << 17)

// Security flags
#define SMB_SECURITY_NO_TRACKING                    0
#define SMB_SECURITY_CONTEXT_TRACKING               (1 << 0)
#define SMB_SECURITY_EFFECTIVE_ONLY                 (1 << 1)

// Write mode flags
#define SMB_WRITEMODE_WRITETHROUGH                  (1 << 0)
#define SMB_WRITEMODE_READBYTESAVAILABLE            (1 << 1)
#define SMB_WRITEMODE_RAW                           (1 << 2)
#define SMB_WRITEMODE_MSG_START                     (1 << 3)

// Trans 2 flags
//// Find First 2
#define SMB_FIND2_ATTR_RO                           (1 << 0)  // Include RO files in result
#define SMB_FIND2_ATTR_HIDDEN                       (1 << 1)  // Include hidden files
#define SMB_FIND2_ATTR_SYSTEM                       (1 << 2)  // Include system files
#define SMB_FIND2_ATTR_VOLUME                       (1 << 3)  // Include volume ID ?
#define SMB_FIND2_ATTR_DIR                          (1 << 4)  // Include directory ?
#define SMB_FIND2_ATTR_ARCHIVE                      (1 << 5)  // Include archive ?
#define SMB_FIND2_ATTR_DEFAULT                      (SMB_FIND2_ATTR_RO | SMB_FIND2_ATTR_HIDDEN |    \
                                                     SMB_FIND2_ATTR_SYSTEM | SMB_FIND2_ATTR_DIR)

#define SMB_FIND2_FLAG_CLOSE                        (1 << 0)  // Close search after request ?
#define SMB_FIND2_FLAG_CLOSE_EOS                    (1 << 1)  // Close after End Of Search ?
#define SMB_FIND2_FLAG_RESUME                       (1 << 2)  // Send resume keys ?
#define SMB_FIND2_FLAG_CONTINUE                     (1 << 3)  // not set == new search
#define SMB_FIND2_FLAG_BACKUP                       (1 << 4)  // Backup intent ?

#endif /* smb_defs_h */
