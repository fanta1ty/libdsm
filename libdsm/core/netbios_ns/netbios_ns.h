//
//  netbios_ns.h
//  libdsm
//
//  Created by trekvn on 4/3/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "netbios_defs.h"

#include <stdint.h>

typedef struct netbios_ns_entry     netbios_ns_entry;

typedef struct netbios_ns netbios_ns;

const char *netbios_ns_entry_name(netbios_ns_entry *entry);

const char *netbios_ns_entry_group(netbios_ns_entry *entry);

uint32_t netbios_ns_entry_ip(netbios_ns_entry *entry);

char netbios_ns_entry_type(netbios_ns_entry *entry);

netbios_ns *netbios_ns_new();

void netbios_ns_destroy(netbios_ns *ns);

int netbios_ns_resolve(netbios_ns *ns, const char *name, char type, uint32_t *addr);

const char *netbios_ns_inverse(netbios_ns *ns, uint32_t ip);

typedef struct {
    void *p_opaque;
    
    void (*pf_on_entry_added) (void *p_opaque, netbios_ns_entry *entry);
    
    void (*pf_on_entry_removed)(void *p_opaque, netbios_ns_entry *entry);
} netbios_ns_discover_callbacks;

int netbios_ns_discover_start(netbios_ns *ns, unsigned int broadcast_timeout,
                              netbios_ns_discover_callbacks *callbacks);

int netbios_ns_discover_stop(netbios_ns *ns);


