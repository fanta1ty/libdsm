//
//  netbios_query.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef netbios_query_h
#define netbios_query_h

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "netbios_defs.h"

typedef struct netbios_query_s {
    size_t                      payload_size;
    size_t                      cursor;
    netbios_query_packet        *packet;
}netbios_query;

netbios_query *netbios_query_new(size_t payload_size, int is_query,
                                 char opcode);

void netbios_query_destroy(netbios_query *q);

void netbios_query_set_flag(netbios_query *q,
                            uint16_t flag, int value);

int netbios_query_append(netbios_query *q, const char *data,
                         size_t data_size);

void netbios_query_print(netbios_query *q);

#endif /* netbios_query_h */
