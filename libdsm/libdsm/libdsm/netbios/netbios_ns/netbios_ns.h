//
//  netbios_ns.h
//  test
//
//  Created by trekvn on 4/7/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef netbios_ns_h
#define netbios_ns_h

#include <stdio.h>

#pragma mark - netbios_ns_entry
typedef struct netbios_ns_entry netbios_ns_entry;

#pragma mark - netbios_ns

typedef struct netbios_ns netbios_ns;

#pragma mark - netbios_ns_discover_callbacks
typedef struct {
    // Opaque pointer that will be passed to callbacks
    void *p_opaque;
    
    // Called when a new entry is added
    void (*pf_on_entry_added) (void *p_opaque, netbios_ns_entry *entry);
    
    // Called when an entry is removed
    void (*pf_on_entry_removed)(void *p_opaque, netbios_ns_entry *entry);
} netbios_ns_discover_callbacks;

#pragma mark - netbios_ns_new
/*!Allocate and initialize the Netbios name service client object.
 *\returns A newly allocated netbios_ns ready for querying.
 */
netbios_ns *netbios_ns_new();

#pragma mark - netbios_ns_entry_ip
/*!Return the IP address of the correspondance referenced by the iterator
 *\returns The ip address of this entry, in network byte order.
 */
uint32_t netbios_ns_entry_ip(netbios_ns_entry *entry);

#pragma mark - netbios_ns_entry_group
/*!Get the name of the entry referenced by the iterator iter.
 * The pointer points to an area of memory owned by the netbios name service
 *\returns A null-terminated ASCII string representing the group of a netbios machine.
 */
const char *netbios_ns_entry_group(netbios_ns_entry *entry);

#pragma mark - netbios_ns_entry_name
/*!Get the name of the entry referenced by the iterator iter.
 * The pointer points to an area of memory owned by the netbios name service
 *\returns A null-terminated ASCII string representing the name of a netbios machine.
 */
const char *netbios_ns_entry_name(netbios_ns_entry *entry);

#pragma mark - netbios_ns_entry_type
/*!Return the type of record
 *\returns The type of netbios record (.ie 0x20 for FileServer, 0 for workstation, etc.) or a value < 0 if the iterator is invalid or an error occured.
 */
char netbios_ns_entry_type(netbios_ns_entry *entry);

#pragma mark - netbios_ns_discover_start
/*!Perform a NETBIOS discovery in a separate thread.
 * This functions starts a new thread that will send a message to '*' Netbios name (broadcast NB query on 255.255.255.255). It'll wait for the machine on the LAN to answer. It'll then perform a reverse lookup on all the ip he received packet from. Once a name and an ip is found, this function will notify the caller by a callback.
 *\params ns The name service object.  @param broadcast_timeout Do a broadcast every timeout seconds @param callbacks The callbacks previously setup by the caller
 *\returns 0 on success or -1 on failure
 */
int netbios_ns_discover_start(netbios_ns *ns, unsigned int broadcast_timeout, netbios_ns_discover_callbacks *callbacks);

#pragma mark - netbios_ns_discover_stop
/*!Stop the NETBIOS discovery.
 *\param ns The name service object.
 *\returns 0 on success or -1 on failure
 */
int netbios_ns_discover_stop(netbios_ns *ns);

#pragma mark - netbios_ns_destroy
/*!Destroy the netbios name service object
 *\param[in] ns A pointer on the netbios_ns to destroy and deallocate
 */
void netbios_ns_destroy(netbios_ns *ns);
#endif /* netbios_ns_h */
