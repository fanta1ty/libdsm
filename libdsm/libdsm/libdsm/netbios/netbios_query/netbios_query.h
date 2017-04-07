//
//  netbios_query.h
//  test
//
//  Created by trekvn on 4/7/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef netbios_query_h
#define netbios_query_h

#include <stdio.h>
#include <stddef.h>

#include "netbios_defs.h"
typedef struct netbios_query_s {
    size_t                      payload_size;
    size_t                      cursor;
    netbios_query_packet        *packet;
} netbios_query;

#pragma mark - netbios_query_new
netbios_query *netbios_query_new(size_t payload_size, int is_query, char opcode);

#pragma mark - netbios_query_set_flag
void netbios_query_set_flag(netbios_query *q, uint16_t flag, int value);

#pragma mark - netbios_query_append
int netbios_query_append(netbios_query *q, const char *data, size_t data_size);
#endif /* netbios_query_h */
