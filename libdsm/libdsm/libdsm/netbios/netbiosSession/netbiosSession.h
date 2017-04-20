//
//  netbiosSession.h
//  test
//
//  Created by trekvn on 4/14/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import "config.h"
#import "netbios_defs.h"

#if !defined _WIN32
#   import <netinet/in.h>
#else
#   import <winsock2.h>
#endif

#define NETBIOS_SESSION_NEW         0
#define NETBIOS_SESSION_CONNECTING  1
#define NETBIOS_SESSION_CONNECTED   2
#define NETBIOS_SESSION_ERROR       -1
#define NETBIOS_SESSION_REFUSED     -2

typedef struct netbios_session_s {
    // The address of the remote peer;
    struct sockaddr_in          remote_addr;
    // The socket of the TCP connection to the HOST'
    int                         socket;
    // The current sessions state; See macro before (eg. NETBIOS_SESSION_ERROR)
    int                         state;
    // What is the size of the allocated payload;
    size_t                      packet_payload_size;
    // Where is the write cursor relative to the beginning of the payload
    size_t                      packet_cursor;
    // Our allocated packet, this is where the magic happen (both send and recv :)
    netbios_session_packet      *packet;
} netbios_session;

@interface netbiosSession : NSObject
#pragma mark - netbiosSessionNew
netbios_session *netbios_session_new(size_t buf_size);

#pragma mark - netbiosSessionDestroy
void netbios_session_destroy(netbios_session *);

#pragma mark - netbiosSessionConnect
int netbios_session_connect(uint32_t ip,
                                          netbios_session *s,
                                          const char *name,
                                          int direct_tcp);

#pragma mark - netbiosSessionPacketInit
void netbios_session_packet_init(netbios_session *s);

#pragma mark - netbiosSessionPacketAppend
int netbios_session_packet_append(netbios_session *s,
                                  const char *data, size_t size);

#pragma mark - netbiosSessionPacketSend
int netbios_session_packet_send(netbios_session *s);

#pragma mark - netbiosSessionPacketRecv
ssize_t netbios_session_packet_recv(netbios_session *s, void **data);
@end
#endif
