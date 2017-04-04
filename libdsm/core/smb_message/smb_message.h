//
//  smb_message.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#ifndef smb_message_h
#define smb_message_h

#include <stdio.h>

#include "smb_defs.h"
#include "smb_types.h"

smb_message *smb_message_new(uint8_t cmd);

#define SMB_MSG_INIT_PKT(pkt) do {                                      \
    memset(&pkt, 0, sizeof(pkt));                                       \
} while (0)

#define SMB_MSG_INIT_PKT_ANDX(pkt) do {                                 \
    SMB_MSG_INIT_PKT(pkt);                                              \
    pkt.andx = 0xff;                                                    \
    pkt.andx_reserved = 0;                                              \
    pkt.andx_offset = 0;                                                \
} while (0)

#define SMB_MSG_PUT_PKT(msg, pkt)                                       \
smb_message_append(msg, &pkt, sizeof(pkt))

#define SMB_MSG_ADVANCE_PKT(msg, pkt)                                   \
smb_message_advance(msg, sizeof(pkt))

#define SMB_MSG_INSERT_PKT(msg, cursor, pkt)                            \
smb_message_insert(msg, cursor, &pkt, sizeof(pkt))
#endif /* smb_message_h */
