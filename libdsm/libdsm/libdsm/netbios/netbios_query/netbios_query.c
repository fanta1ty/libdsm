//
//  netbios_query.c
//  test
//
//  Created by trekvn on 4/7/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#include "config.h"
#include "netbios_query.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#ifdef HAVE_ARPA_INET_H
#   include <arpa/inet.h>
#endif

#ifdef _WIN32
#   include <winsock2.h>
#endif

#pragma mark - netbios_query_set_flag
void netbios_query_set_flag(netbios_query *q, uint16_t flag, int value) {
    assert(q && q->packet);
    
    if (value) {
        q->packet->flags = htons(ntohs(q->packet->flags) | flag);
    } else {
        q->packet->flags = htons(ntohs(q->packet->flags) & ~flag);
    }
}

#pragma mark - netbios_query_new
netbios_query *netbios_query_new(size_t payload_size, int is_query, char opcode) {
    netbios_query *q;
    
    q = calloc(1, sizeof(netbios_query));
    if (!q)
        return NULL;
    
    q->packet = calloc(1, sizeof(netbios_query_packet) + payload_size);
    if (!q->packet) {
        free(q);
        return NULL;
    }
    
    q->payload_size = payload_size;
    
    q->packet->flags  = htons(opcode << 11);
    netbios_query_set_flag(q, NETBIOS_FLAG_QUERY, !is_query);
    
    return q;
}

#pragma mark - netbios_query_append
int netbios_query_append(netbios_query *q, const char *data, size_t data_size) {
    assert(q && q->packet);
    
    if (q->payload_size - q->cursor < data_size) {
        return -1;
    }
    
    memcpy(((char *)&q->packet->payload) + q->cursor, data, data_size);
    q->cursor += data_size;
    return 0;
}
