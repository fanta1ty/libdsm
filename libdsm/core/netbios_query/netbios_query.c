//
//  netbios_query.c
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#include "netbios_query.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif
#ifdef _WIN32
# include <winsock2.h>
#endif


netbios_query *netbios_query_new(size_t payload_size, int is_query,
                                 char opcode) {
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

void netbios_query_destroy(netbios_query *q) {
    assert(q);
    
    free(q->packet);
    free(q);
}

void netbios_query_set_flag(netbios_query *q,
                            uint16_t flag, int value) {
    assert(q && q->packet);
    
    if (value)
        q->packet->flags = htons(ntohs(q->packet->flags) | flag);
    else
        q->packet->flags = htons(ntohs(q->packet->flags) & ~flag);
}

int netbios_query_append(netbios_query *q, const char *data,
                         size_t data_size) {
    assert(q && q->packet);
    
    if (q->payload_size - q->cursor < data_size)
        return -1;
    
    memcpy(((char *)&q->packet->payload) + q->cursor, data, data_size);
    q->cursor += data_size;
    return 0;
}

void netbios_query_print(netbios_query *q) {
    assert(q && q->packet);
    
    printf("--- netbios_query dump :\n");
    printf("payload = %zu, cursor = %zu.\n", q->payload_size, q->cursor);
    printf("Transaction id = %u.\n", q->packet->trn_id);
    printf("-------------------------\n");
    
    for (unsigned i = 0; i < sizeof(netbios_query_packet) + q->cursor; i++) {
        char c;
        if ((i % 8) == 0 && i != 0)
            printf("\n");
        if ((i % 8) == 0)
            printf("0x");
        
        c = ((char *)q->packet)[i];
        printf("%.2hhX ", c);
    }
    printf("\n");
    printf("-------------------------\n");
}
