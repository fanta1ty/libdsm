//
//  smb_transport.c
//  test
//
//  Created by trekvn on 4/10/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#include "config.h"
#include "smb_transport.h"
#include "netbios_session.h"

#include <assert.h>

int transport_connect_tcp(uint32_t ip, netbios_session *s, const char *name) {
    return netbios_session_connect(ip, s, name, 1);
}

int transport_connect_nbt(uint32_t ip, netbios_session *s, const char *name) {
    return netbios_session_connect(ip, s, name, 0);
}

#pragma mark - smb_transport_nbt
void smb_transport_nbt(smb_transport *tr) {
    assert(tr != NULL);
    
    // Sorry for the dirty cast.
    tr->new = (void *)netbios_session_new;
    tr->connect = (void *)transport_connect_nbt;
    tr->destroy = (void *)netbios_session_destroy;
    tr->pkt_init = (void *)netbios_session_packet_init;
    tr->pkt_append = (void *)netbios_session_packet_append;
    tr->send = (void *)netbios_session_packet_send;
    tr->recv = (void *)netbios_session_packet_recv;
}

#pragma mark - smb_transport_tcp
void smb_transport_tcp(smb_transport *tr) {
    assert(tr != NULL);
    
    tr->new = (void *)netbios_session_new;
    tr->connect = (void *)transport_connect_tcp;
    tr->destroy = (void *)netbios_session_destroy;
    tr->pkt_init = (void *)netbios_session_packet_init;
    tr->pkt_append = (void *)netbios_session_packet_append;
    tr->send = (void *)netbios_session_packet_send;
    tr->recv = (void *)netbios_session_packet_recv;
}

