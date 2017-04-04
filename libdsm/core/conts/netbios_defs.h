//
//  netbios_defs.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef netbios_defs_h
#define netbios_defs_h

#include <stdint.h>
#include "bdsm_common.h"

/*!Netbios name types */
#define NETBIOS_WORKSTATION             0x00
#define NETBIOS_MESSENGER               0x03
#define NETBIOS_FILESERVER              0x20
#define NETBIOS_DOMAINMASTER            0x1b

#define NETBIOS_PORT_NAME               137     // UDP
#define NETBIOS_PORT_SESSION            139     // TCP
#define NETBIOS_PORT_DIRECT             445     // TCP
#define NETBIOS_PORT_DIRECT_SECONDARY   139     // TCP

#define NETBIOS_NAME_LENGTH             15

#define NETBIOS_NAME_FLAG_GROUP         (1 << 15)

#define NETBIOS_WILDCARD      { 32, 'C', 'K', 'A', 'A', 'A', 'A', 'A', 'A',         \
                                'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',        \
                                'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',        \
                                'A', 'A', 'A', 'A', 'A', 'A', 0 }

#define NETBIOS_FLAG_QUERY              (1 << 15)
#define NETBIOS_FLAG_TRUNCATED          (1 << 9)
#define NETBIOS_FLAG_RECURSIVE          (1 << 8)
#define NETBIOS_FLAG_BROADCAST          (1 << 4)

// Name Service Query
#define NETBIOS_OP_NAME_QUERY           0x00

// Session Service
#define NETBIOS_OP_SESSION_MSG          0x00
#define NETBIOS_OP_SESSION_REQ          0x81
#define NETBIOS_OP_SESSION_REQ_OK       0x82
#define NETBIOS_OP_SESSION_REQ_NOK      0x83
#define NETBIOS_OP_SESSION_RETARGET     0x84
#define NETBIOS_OP_SESSION_KEEPALIVE    0x85

SMB_PACKED_START typedef struct {
    uint16_t                    trn_id;     // Transaction ID
    uint16_t                    flags;      // Various flags
    uint16_t                    queries;    // Number of queries in this packet
    uint16_t                    answers;    // Number of answers
    uint16_t                    ns_count;   // Number of authorities (?)
    uint16_t                    ar_count;   // Additionnal (??)
    char                        payload[];
} SMB_PACKED_END   netbios_query_packet;

SMB_PACKED_START typedef struct {
    uint8_t                     opcode;     // 'TYPE'
    uint8_t                     flags;      // 0-6 reserved (== 0), byte 7 is the
                                            // beginning of the length field (!!)
    uint16_t                    length;     // payload length;
    uint8_t                     payload[];
} SMB_PACKED_END   netbios_session_packet;

#endif /* netbios_defs_h */
