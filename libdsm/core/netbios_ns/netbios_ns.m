//
//  netbios_ns.m
//  libdsm
//
//  Created by trekvn on 4/3/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import "netbios_ns.h"
#import "bdsm_debug.h"
#import "netbios_query.h"
#import "netbios_utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#include <sys/queue.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <net/if.h>


#ifdef HAVE_SYS_QUEUE_H
# include <sys/queue.h>
#endif
#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif
#ifdef _WIN32
# include <winsock2.h>
# include <ws2tcpip.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif

#ifndef _WIN32
# include <sys/types.h>
# ifdef HAVE_IFADDRS_H
#  include <ifaddrs.h>
# endif
# include <net/if.h>
#endif


//#ifdef __BDSM_NETBIOS_NS_H_
//#define __BDSM_NETBIOS_NS_H_

enum name_query_type {
    NAME_QUERY_TYPE_INVALID,
    NAME_QUERY_TYPE_NB,
    NAME_QUERY_TYPE_NBSTAT
};

static char name_query_broadcast[] = NETBIOS_WILDCARD;

enum ns_entry_flag {
    NS_ENTRY_FLAG_INVALID = 0x00,
    NS_ENTRY_FLAG_VALID_IP = 0x01,
    NS_ENTRY_FLAG_VALID_NAME = 0x02
};

struct netbios_ns_entry {
    TAILQ_ENTRY(netbios_ns_entry)       next;
    char                                name[NETBIOS_NAME_LENGTH + 1];
    char                                group[NETBIOS_NAME_LENGTH + 1];
    char                                type;
    int                                 flag;
    time_t                              last_time_seen;
};

typedef TAILQ_HEAD(, netbios_ns_entry) NS_ENTRY_QUEUE;

#define RECV_BUFFER_SIZE                1500 /*!-> Max MTU frame size for ethernet */

struct netbios_ns {
    int                             socket;
    struct sockaddr_in              addr;
    uint16_t                        last_trn_id; /*!-> Last transaction id used */
    NS_ENTRY_QUEUE                  entry_queue;
    uint8_t                         buffer[RECV_BUFFER_SIZE];
    
#ifdef HAVE_PIPE
    int                             abort_pipe[2];
#else
    pthread_mutex_t                 abort_lock;
    bool                            aborted;
#endif
    unsigned int                    discover_broadcast_timeout;
    pthread_t                       discover_thread;
    bool                            discover_started;
    netbios_ns_discover_callbacks   discover_callbacks;
};

typedef struct netbios_ns_name_query    netbios_ns_name_query;
struct netbios_ns_name_query {
    enum name_query_type    type;
    union {
        struct {
            uint32_t ip;
        } nb;
        
        struct {
            const char      *name;
            const char      *group;
            char            type;
        } nbstat;
    }u;
};

#pragma mark - methods
static int ns_open_socket(netbios_ns *ns) {
    int sock_opt;
    
    if ((ns->socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        goto error;
    }
    
    sock_opt = 0;
    
    if (setsockopt(ns->socket, IPPROTO_IP,
                   IP_MULTICAST_LOOP, (void *)&sock_opt,
                   sizeof(sock_opt)) < 0) {
        goto error;
    }
    
    ns->addr.sin_family = AF_INET;
    ns->addr.sin_port = htons(0);
    ns->addr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(ns->socket, (struct sockaddr *)&ns->addr, sizeof(ns->addr)) < 0) {
        goto error;
    }
    return 1;
    
error:
    bdsm_dbg("netbios_ns_new, open_socket: ");
    return 0;
}

#ifdef HAVE_PIPE
static int ns_open_abort_pipe(netbios_ns *ns) {
    int flags;
    
    if (pipe(ns->abort_pipe) == -1)
        return -1;
    
    #ifndef _WIN32
    if ((flags = fcntl(ns->abort_pipe[0], F_GETFL, 0)) == -1)
        return -1;
    
    if (fcntl(ns->abort_pipe[0], F_SETFL, flags | O_NONBLOCK) == -1)
        return -1;
    #endif
    
    return 0;
}

static void ns_close_abort_pipe(netbios_ns *ns) {
    if (ns->abort_pipe[0] != -1 &&
        ns->abort_pipe[1] != -1) {
        
        closesocket(ns->abort_pipe[0]);
        closesocket(ns->abort_pipe[1]);
        ns->abort_pipe[0] = ns->abort_pipe = -1;
    }
}

static bool netbios_ns_is_aborted(netbios_ns *ns) {
    fd_set read_fds;
    int res;
    struct timeval timeout = {0, 0};
    FD_ZERO(&read_fds);
    
    FD_SET(ns->abort_pipe[0], &read_fds);
    
    res = select(ns->abort_pipe[0] + 1, &read_fds, NULL, NULL, &timeout);
    
    return (res < 0 || FD_ISSET(ns->abort_pipe[0], &read_fds));
}

static void netbios_ns_abort(netbios_ns *ns) {
    uint8_t buf = '\0';
    write(ns->abort_pipe[1], &buf, sizeof(uint8_t));
}
#else
static int ns_open_abort_pipe(netbios_ns *ns) {
    return pthread_mutex_init(&ns->abort_lock, NULL);
}

static void ns_close_abort_pipe(netbios_ns *ns) {
    pthread_mutex_destroy(&ns->abort_lock);
}

static bool netbios_ns_is_aborted(netbios_ns *ns) {
    pthread_mutex_lock(&ns->abort_lock);
    bool res = ns->aborted;
    pthread_mutex_unlock(&ns->abort_lock);
    return res;
}

static void netbios_ns_abort(netbios_ns *ns) {
    pthread_mutex_lock(&ns->abort_lock);
    ns->aborted = true;
    pthread_mutex_unlock(&ns->abort_lock);
}
#endif

static uint16_t query_type_nb = 0x2000;
static uint16_t query_type_nbstat = 0x2100;
static uint16_t query_class_in = 0x0100;

static ssize_t netbios_ns_send_packet(netbios_ns *ns, netbios_query *q, uint32_t ip) {
    struct sockaddr_in addr;
    
    addr.sin_addr.s_addr = ip;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(NETBIOS_PORT_NAME);
    
    bdsm_dbg("Sending netbios packet to %s\n", inet_ntoa(addr.sin_addr));
    
    return sendto(ns->socket, (void *)q->packet,
                  sizeof(netbios_query_packet) + q->cursor, 0,
                  (struct sockaddr *)&addr,
                  sizeof(struct sockaddr_in));
}

static void netbios_ns_broadcast_packet(netbios_ns *ns, netbios_query *q) {
    netbios_ns_send_packet(ns, q, INADDR_BROADCAST);
}

static int netbios_ns_send_name_query(netbios_ns *ns,
                                      uint32_t ip,
                                      enum name_query_type type,
                                      const char *name,
                                      uint16_t query_flag) {
    uint16_t query_type;
    netbios_query *q;
    
    assert(ns != NULL);
    
    switch (type) {
        case NAME_QUERY_TYPE_NB: {
            query_type = query_type_nb;
            break;
        }
            
        case NAME_QUERY_TYPE_NBSTAT: {
            query_type = query_type_nbstat;
            break;
        }
            
        default: {
            bdsm_dbg("netbios_ns_send_name_query: unknow name_query_type");
            return -1;
            break;
        }
    }
    
    /*!Prepare packet */
    q = netbios_query_new(34 + 4, 1, NETBIOS_OP_NAME_QUERY);
    
    if (query_flag) {
        netbios_query_set_flag(q, query_flag, 1);
    }
    
    /*!Append the queried name to the packet */
    netbios_query_append(q, name, strlen(name) + 1);
    
    /*!Magic footer */
    netbios_query_append(q, (const char *)&query_type, 2);
    netbios_query_append(q, (const char *)&query_class_in, 2);
    q->packet->queries = htons(1);
    
    /*!Increment transaction ID, not to reuse them */
    q->packet->trn_id = htons(ns->last_trn_id + 1);
    
    if (ip != 0) {
        ssize_t sent = netbios_ns_send_packet(ns, q, ip);
        if (sent < 0) {
            netbios_query_destroy(q);
            return -1;
        } else {
            bdsm_dbg("netbios_ns_send_name_query, name query send for '*'.\n");
        }
    } else {
        netbios_ns_broadcast_packet(ns, q);
    }
    
    netbios_query_destroy(q);
    ns->last_trn_id ++;
    return 0;
}

static int netbios_ns_handle_query(netbios_ns *ns, size_t size,
                                   bool check_trn_id, uint32_t recv_ip,
                                   netbios_ns_name_query *out_name_query) {
    netbios_query_packet *q;
    uint8_t name_size;
    uint16_t *p_type, type;
    uint16_t *p_data_length, data_length;
    char *p_data;
    
    /*!Check for packet size */
    if (size < sizeof(netbios_query_packet)) {
        bdsm_dbg("netbios_ns_handle_query, invalid size. \n");
        return -1;
    }
    
    q = (netbios_query_packet *)ns->buffer;
    
    if (check_trn_id) {
        /*!Check if trn_id corresponds */
        if (ntohs(q->trn_id) != ns->last_trn_id) {
            bdsm_dbg("netbios_ns_handle_query, invalid trn_id: %d vs %d \n",
                     ntohs(q->trn_id), ns->last_trn_id);
            return -1;
        }
    }
    
    if (!out_name_query) {
        return 0;
    }
    
    /*!Get name size, should be 0x20 */
    if (size < sizeof(netbios_query_packet) + 1) {
        return -1;
    }
    
    name_size = q->payload[0];
    if (name_size != 0x20) {
        return -1;
    }
    
    /*!Get type and data_length */
    if (size < sizeof(netbios_query_packet) + name_size + 11) {
        return -1;
    }
    
    p_type = (uint16_t *)(q->payload + name_size + 2);
    type = *p_type;
    p_data_length = (uint16_t *)(q->payload + name_size + 10);
    data_length = ntohs(*p_data_length);
    
    if (size < sizeof(netbios_query_packet) + name_size + 12 + data_length) {
        return -1;
    }
    p_data = q->payload + name_size + 12;
    
    if (type == query_type_nb) {
        out_name_query->type = NAME_QUERY_TYPE_NB;
        out_name_query->u.nb.ip = recv_ip;
    } else if (type == query_type_nbstat) {
        uint8_t name_count;
        const char *names = NULL;
        const char *group = NULL, *name = NULL;
        
        /*!Get the number of names */
        if (data_length < 1) {
            return -1;
        }
        name_count = *(p_data);
        names = p_data + 1;
        
        if (data_length < name_count * 18) {
            return -1;
        }
        
        /*!First search for a group in the name list */
        for (uint8_t name_idx = 0; name_idx < name_count; name_idx ++) {
            const char *current_name = names + name_idx * 18;
            uint16_t current_flags = (current_name[16] << 8) | current_name[17];
            
            if (current_flags & NETBIOS_NAME_FLAG_GROUP) {
                group = current_name;
                break;
            }
        }
        
        /*!Then search for file servers */
        for (uint8_t name_idx = 0; name_idx < name_count; name_idx ++) {
            const char *current_name = names + name_idx * 18;
            char current_type = current_name[15];
            uint16_t current_flags = (current_name[16] << 8) | current_name[17];
            
            if (current_flags & NETBIOS_NAME_FLAG_GROUP) {
                continue;
            }
            if (current_type == NETBIOS_FILESERVER) {
                name = current_name;
                bdsm_dbg("netbios_ns_handle_query, Found name: '%.*s' in group: '%.*s'\n",
                         NETBIOS_NAME_LENGTH, name, NETBIOS_NAME_LENGTH, group);
                break;
            }
        }
        
        if (name) {
            out_name_query->type = NAME_QUERY_TYPE_NBSTAT;
            out_name_query->u.nbstat.name = name;
            out_name_query->u.nbstat.group = group;
            out_name_query->u.nbstat.type = NETBIOS_FILESERVER;
        }
    }
    
    return 0;
}

static ssize_t netbios_ns_recv(netbios_ns *ns,
                               struct timeval *timeout,
                               struct sockaddr_in *out_addr,
                               bool check_trn_id,
                               uint32_t wait_ip,
                               netbios_ns_name_query *out_name_query) {
    int sock;
    
    assert(ns != NULL);
    
    sock = ns->socket;
#ifdef HAVE_PIPE
    int abort_fd =  ns->abort_pipe[0];
#else
    int abort_fd = -1;
#endif
    
    if (out_name_query)
        out_name_query->type = NAME_QUERY_TYPE_INVALID;
    
    while (true) {
        fd_set read_fds, error_fds;
        int res, nfds;
        
        FD_ZERO(&read_fds);
        FD_ZERO(&error_fds);
        FD_SET(sock, &read_fds);
#ifdef HAVE_PIPE
        FD_SET(abort_fd, &read_fds);
#endif
        FD_SET(sock, &error_fds);
        nfds = (sock > abort_fd ? sock : abort_fd) + 1;
        
        res = select(nfds, &read_fds, 0, &error_fds, timeout);
        
        if (res < 0)
            goto error;
        if (FD_ISSET(sock, &error_fds))
            goto error;
        
#ifdef HAVE_PIPE
        if (FD_ISSET(abort_fd, &read_fds))
            return -1;
#else
        if (netbios_ns_is_aborted(ns))
            return -1;
#endif
        else if (FD_ISSET(sock, &read_fds)) {
            struct sockaddr_in addr;
            socklen_t addr_len = sizeof(struct sockaddr_in);
            ssize_t size;
            
            size = recvfrom(sock, ns->buffer, RECV_BUFFER_SIZE, 0,
                            (struct sockaddr *)&addr, &addr_len);
            if (size < 0)
                return -1;
            if (wait_ip != 0 && addr_len >= sizeof(struct sockaddr_in)) {
                /*! Wait for a reply from a specific ip */
                if (wait_ip != addr.sin_addr.s_addr) {
                    bdsm_dbg("netbios_ns_recv, invalid ip");
                    continue;
                }
            }
            
            if (netbios_ns_handle_query(ns, (size_t)size, check_trn_id,
                                        addr.sin_addr.s_addr,
                                        out_name_query) == -1) {
                bdsm_dbg("netbios_ns_recv, invalid query\n");
                continue;
            }
            
            if (out_addr)
                *out_addr = addr;
            return size;
        }
        else
            return 0;
    }
    
error:
    return -1;
}
