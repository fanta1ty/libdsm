//
//  netbios_defs.h
//  test
//
//  Created by trekvn on 4/7/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef netbios_defs_h
#define netbios_defs_h
#include "bdsm_common.h"

#define NETBIOS_NAME_LENGTH             15

// Netbios name types
#define NETBIOS_WORKSTATION             0x00
#define NETBIOS_MESSENGER               0x03
#define NETBIOS_FILESERVER              0x20
#define NETBIOS_DOMAINMASTER            0x1b

SMB_PACKED_START typedef struct {
    uint16_t                    trn_id;     // Transaction ID
    uint16_t                    flags;      // Various flags
    uint16_t                    queries;    // Number of queries in this packet
    uint16_t                    answers;    // Number of answers
    uint16_t                    ns_count;   // Number of authorities (?)
    uint16_t                    ar_count;   // Additionnal (??)
    char                        payload[];
} SMB_PACKED_END netbios_query_packet;

#define NETBIOS_FLAG_QUERY              (1 << 15)
#define NETBIOS_NAME_FLAG_GROUP         (1 << 15)

// Name Service Query
#define NETBIOS_OP_NAME_QUERY           0x00

#define NETBIOS_PORT_NAME               137     // UDP

#define NETBIOS_WILDCARD      { 32, 'C', 'K', 'A', 'A', 'A', 'A', 'A', 'A',    \
    'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', \
    'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 0 }

#endif /* netbios_defs_h */
