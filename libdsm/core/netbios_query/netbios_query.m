//
//  netbios_query.m
//  libdsm
//
//  Created by trekvn on 4/3/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import "netbios_query.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <arpa/inet.h>

netbios_query *netbios_query_new(size_t payload_size, int is_query, char opcode) {
    netbios_query *q;
    
    q = calloc(1, sizeof(netbios_query));
    if (!q) {
        return NULL;
    }
    
    q->packet = calloc(1, sizeof(netbios_query_packet) + payload_size);
    if (!q->packet) {
        free(q);
        return NULL;
    }

    q->payload_size = payload_size;
    q->packet->flags = htons(opcode << 11);
    
    netbios_query_set_flag(q, NETBIOS_FLAG_QUERY, !is_query);
    
    return q;
}

void netbios_query_destroy(netbios_query *q) {
    assert(q);
    
    free(q->packet);
    free(q);
}

void netbios_query_set_flag(netbios_query *q, uint16_t flag, int value) {
    assert(q && q->packet);
    
    if (value) {
        q->packet->flags = htons(ntohs(q->packet->flags) | flag);
    } else {
        q->packet->flags = htons(ntohs(q->packet->flags) & ~flag);
    }
}

int netbios_query_append(netbios_query *q, const char *data, size_t data_size) {
    assert(q && q->packet);
    
    if (q->payload_size - q->cursor < data_size) {
        return -1;
    }
    memcpy(((char *)&q->packet->payload) + q->cursor, data, data_size);
    q->cursor += data_size;
    return 0;
}

void netbios_query_print(netbios_query *q) {
    assert(q && q->packet);
    
    NSLog(@"--netbios_query dump:\n");
    NSLog(@"payload = %zu, cursor: %zu.\n", q->payload_size, q->cursor);
    NSLog(@"transaction id = %u.\n", q->packet->trn_id);
    NSLog(@"-------------------------\n");
    
    for (unsigned i = 0; i < sizeof(netbios_query_packet) + q->cursor; i++) {
        char c;
        if ((i % 8) == 0 && i != 0) {
            NSLog(@"\n");
        }
        if ((i % 8) == 0) {
            NSLog(@"0x");
        }
        
        c = ((char *)q->packet)[i];
        NSLog(@"%.2hhX", c);
    }
    NSLog(@"\n");
    NSLog(@"-------------------------\n");
}


