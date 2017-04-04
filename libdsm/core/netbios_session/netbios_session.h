//
//  netbios_session.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef netbios_session_h
#define netbios_session_h

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <netinet/in.h>

#include "netbios_defs.h"

#if !defined _WIN32
#   include <netinet/in.h>
#else
#   include <winsock2.h>
#endif

#define NETBIOS_SESSION_NEW             0
#define NETBIOS_SESSION_CONNECTING      1
#define NETBIOS_SESSION_CONNECTED       2
#define NETBIOS_SESSION_ERROR           -1
#define NETBIOS_SESSION_REFUSED         -2

typedef struct netbios_session_s {
    struct sockaddr_in          remote_addr;            // The address of the remote peer;
    int                         socket;                 // The socket of the TCP connection to the HOST'
    int                         state;                  // The current sessions state; See macro before
                                                        // (eg. NETBIOS_SESSION_ERROR)
    size_t                      packet_payload_size;    // What is the size of the allocated payload;
    size_t                      packet_cursor;          // Where is the write cursor relative to the
                                                        // beginning of the payload
    netbios_session_packet      *packet;                // Our allocated packet, this is where the
                                                        // magic happen (both send and recv :)
}netbios_session;

/*!Return NULL if unable to open socket/connect
 */
netbios_session *netbios_session_new(size_t buf_size);

void netbios_session_destroy(netbios_session *s);

int netbios_session_connect(uint32_t ip, netbios_session *s,
                            const char *name, int direct_tcp);

void netbios_session_packet_init(netbios_session *s);

int netbios_session_packet_append(netbios_session *s,
                                  const char *data, size_t size);

int netbios_session_packet_send(netbios_session *s);

ssize_t netbios_session_packet_recv(netbios_session *s, void **data);

#endif /* netbios_session_h */
