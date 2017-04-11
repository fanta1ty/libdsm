//
//  smb_message.h
//  test
//
//  Created by trekvn on 4/10/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef smb_message_h
#define smb_message_h

#include "smb_defs.h"
#include "smb_types.h"
#include <stdio.h>

smb_message     *smb_message_new(uint8_t cmd);
smb_message     *smb_message_grow(smb_message *msg, size_t size);
void            smb_message_destroy(smb_message *msg);
int             smb_message_advance(smb_message *msg, size_t size);
int             smb_message_append(smb_message *msg, const void *data,
                                   size_t data_size);
int             smb_message_insert(smb_message *msg, size_t cursor,
                                   const void *data, size_t data_size);
int             smb_message_put8(smb_message *msg, uint8_t data);
int             smb_message_put16(smb_message *msg, uint16_t data);
int             smb_message_put32(smb_message *msg, uint32_t data);
int             smb_message_put64(smb_message *msg, uint64_t data);
size_t          smb_message_put_utf16(smb_message *msg, const char *str,
                                      size_t str_len);
int             smb_message_put_uuid(smb_message *msg, uint32_t a, uint16_t b,
                                     uint16_t c, const uint8_t e[8]);

void            smb_message_set_andx_members(smb_message *msg);
void            smb_message_flag(smb_message *msg, uint32_t flag, int value);

#define SMB_MSG_INIT_PKT(pkt) do { \
memset(&pkt, 0, sizeof(pkt)); \
} while (0)

#define SMB_MSG_INIT_PKT_ANDX(pkt) do { \
SMB_MSG_INIT_PKT(pkt); \
pkt.andx           = 0xff; \
pkt.andx_reserved  = 0; \
pkt.andx_offset    = 0; \
} while (0)

#define SMB_MSG_PUT_PKT(msg, pkt) \
smb_message_append(msg, &pkt, sizeof(pkt))

#define SMB_MSG_ADVANCE_PKT(msg, pkt) \
smb_message_advance(msg, sizeof(pkt))

#define SMB_MSG_INSERT_PKT(msg, cursor, pkt) \
smb_message_insert(msg, cursor, &pkt, sizeof(pkt))
#endif /* smb_message_h */
