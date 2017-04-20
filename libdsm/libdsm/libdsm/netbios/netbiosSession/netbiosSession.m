//
//  netbiosSession.m
//  test
//
//  Created by trekvn on 4/14/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import "netbiosSession.h"
#import "config.h"
#import "smb_defs.h"
#import "compat.h"
#import "netbiosHeader.h"

#import <assert.h>
#import <stdio.h>
#import <stdlib.h>
#import <string.h>
#import <unistd.h>
#import <stdbool.h>

#ifdef HAVE_ARPA_INET_H
#   import <arpa/inet.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#   import <sys/socket.h>
#endif
#   import <errno.h>

@implementation netbiosSession
#pragma mark - netbiosSessionNew
netbios_session *netbios_session_new(size_t buf_size) {
    netbios_session *session;
    size_t packet_size;
    
    session = (netbios_session *)calloc(1, sizeof(netbios_session));
    if (!session) {
        return NULL;
    }
    
    session->packet_payload_size = buf_size;
    packet_size = sizeof(netbios_session_packet) + session->packet_payload_size;
    session->packet = (netbios_session_packet *)malloc(packet_size);
    if (!session->packet) {
        free(session);
        return NULL;
    }
    session->socket = -1;
    
    return session;
}

#pragma mark - netbiosSessionDestroy
void netbios_session_destroy(netbios_session * s) {
    if (!s) {
        return;
    }
    
    if (s->socket != -1) {
        closesocket(s->socket);
    }
    
    free(s->packet);
    free(s);
}

static int open_socket_and_connect(netbios_session *s) {
    if ((s->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        goto error;
    }
    
    if (connect(s->socket, (struct sockaddr *)&s->remote_addr, sizeof(s->remote_addr)) <0) {
        goto error;
    }
    
    return DSM_SUCCESS;
    
error:
    //bdsm_perror("netbios_session_new, open_socket: ");
    return DSM_ERROR_NETWORK;
}

#pragma mark - netbiosSessionConnect
int netbios_session_connect(uint32_t ip, netbios_session *s, const char *name, int direct_tcp) {
    ssize_t recv_size;
    char *encoded_name = NULL;
    uint16_t ports[2];
    unsigned int nb_ports;
    bool opened = false;
    
    assert(s != NULL && s->packet != NULL);
    
    if (direct_tcp) {
        ports[0] = htons(NETBIOS_PORT_DIRECT);
        ports[1] = htons(NETBIOS_PORT_DIRECT_SECONDARY);
        nb_ports = 2;
    } else {
        ports[0] = htons(NETBIOS_PORT_SESSION);
        nb_ports = 1;
    }
    
    for (unsigned int i = 0; i < nb_ports && !opened; ++i) {
        s->remote_addr.sin_port         = ports[i];
        s->remote_addr.sin_family       = AF_INET;
        s->remote_addr.sin_addr.s_addr  = ip;
        opened = open_socket_and_connect(s) == DSM_SUCCESS;
    }
    
    if (!opened) {
        goto error;
    }
    
    if (!direct_tcp) {
        // Send the Session Request message
        netbios_session_packet_init(s);
        s->packet->opcode = NETBIOS_OP_SESSION_REQ;
        encoded_name = netbios_name_encode(name, 0, NETBIOS_FILESERVER);
        if (!netbios_session_packet_append(s, encoded_name, strlen(encoded_name) + 1)) {
            goto error;
        }
        free(encoded_name);
        encoded_name = netbios_name_encode("LIBDSM", 0, NETBIOS_WORKSTATION);
        if (!netbios_session_packet_append(s, encoded_name, strlen(encoded_name) + 1))
            goto error;
        free(encoded_name);
        encoded_name = NULL;
        
        s->state = NETBIOS_SESSION_CONNECTING;
        if (!netbios_session_packet_send(s))
            goto error;
        
        // Now receiving the reply from the server.
        recv_size = netbios_session_packet_recv(s, NULL);
        if (recv_size < 0)
            goto error;
        
        // Reply was negative, we are not connected :(
        if (s->packet->opcode != NETBIOS_OP_SESSION_REQ_OK) {
            s->state = NETBIOS_SESSION_REFUSED;
            return 0;
        }
    }
    
    // Reply was OK or DirectTCP, a session has been established
    s->state = NETBIOS_SESSION_CONNECTED;
    return 1;
    
error:
    free(encoded_name);
    s->state = NETBIOS_SESSION_ERROR;
    return 0;
}

#pragma mark - netbiosSessionPacketInit
void netbios_session_packet_init(netbios_session *s) {
    assert(s != NULL);
    
    s->packet_cursor = 0;
    s->packet->flags = 0;
    s->packet->opcode = NETBIOS_OP_SESSION_MSG;
}

#pragma mark - netbiosSessionPacketAppend
int netbios_session_packet_append(netbios_session *s, const char *data, size_t size) {
    char *start;
    
    assert(s && s->packet);
    
    if (s->packet_payload_size - s->packet_cursor < size) {
        if (!session_buffer_realloc(s, size + s->packet_cursor)) {
            return 0;
        }
    }
    
    start = ((char *)&s->packet->payload) + s->packet_cursor;
    memcpy(start, data, size);
    s->packet_cursor += size;
    
    return 1;
}

#pragma mark - netbiosSessionPacketSend
int netbios_session_packet_send(netbios_session *s) {
    ssize_t to_send;
    ssize_t sent;
    
    assert(s && s->packet && s->socket >= 0 && s->state > 0);
    
    s->packet->length = htons(s->packet_cursor);
    to_send = sizeof(netbios_session_packet) + s->packet_cursor;
    sent = send(s->socket, (void *)s->packet, to_send, 0);
    
    if (sent != to_send) {
        //bdsm_perror("netbios_session_packet_send: Unable to send (full?) packet");
        return 0;
    }
    
    return sent;
}

static int session_buffer_realloc(netbios_session *s, size_t new_size) {
    void *new_ptr;
    
    assert(s != NULL);
    
    new_ptr  = realloc(s->packet, new_size);
    if (new_ptr != NULL) {
        s->packet_payload_size = new_size;
        s->packet = new_ptr;
        return 1;
    }
    
    free(s->packet);
    s->packet = NULL;
    return 0;
}

static ssize_t netbios_session_get_next_packet(netbios_session *s) {
    ssize_t res;
    size_t total;
    size_t sofar;
    
    assert(s != NULL && s->packet != NULL && s->socket >= 0 && s->state > 0);
    
    // Only get packet header and analyze it to get only needed number of bytes
    // needed for the packet. This will prevent losing a part of next packet
    total = sizeof(netbios_session_packet);
    sofar = 0;
    while (sofar < total) {
        res = recv(s->socket, (uint8_t *)(s->packet) + sofar, total - sofar, 0);
        if (res <= 0) {
            //bdsm_perror("netbios_session_packet_recv: ");
            return -1;
        }
        sofar += res;
    }
    
    total  = ntohs(s->packet->length);
    total |= (s->packet->flags & 0x01) << 16;
    sofar  = 0;
    
    if (total + sizeof(netbios_session_packet) > s->packet_payload_size &&
        !session_buffer_realloc(s, total + sizeof(netbios_session_packet))) {
        return -1;
    }
    
    while (sofar < total) {
        res = recv(s->socket, (uint8_t *)(s->packet) + sizeof(netbios_session_packet)
                   + sofar, total - sofar, 0);
        if (res <= 0) {
            return -1;
        }
        sofar += res;
    }
    
    if (sofar > total) {
        return -1;
    }
    
    return sofar;
}

#pragma mark - netbiosSessionPacketRecv
ssize_t netbios_session_packet_recv(netbios_session *s, void **data) {
    ssize_t size;
    
    // ignore keepalive messages if needed
    do {
        size = netbios_session_get_next_packet(s);
    } while (size >= 0 && s->packet->opcode == NETBIOS_OP_SESSION_KEEPALIVE);
    
    if ((size >= 0) && (data != NULL)) {
        *data = (void *) s->packet->payload;
    }
    
    return size;
}
@end
