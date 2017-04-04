//
//  smbDiscovery.m
//  libdsm
//
//  Created by trekvn on 4/3/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import "smbDiscovery.h"
#import "smbDevice.h"
#import "netbios_ns.h"
#import "netbios_defs.h"

#import <netdb.h>
#import <arpa/inet.h>

@implementation smbDiscovery {
    netbios_ns *_nameServices;
}

static void (^_addedHandler)(smbDevice *_Nonnull);
static void (^_removedHandler)(smbDevice *_Nonnull);
static smbDeviceType _typeMask;

+ (instancetype)sharedInstance {
    static id sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

- (void)dealloc {
    [self stopDiscovery];
}

#pragma mark - startDiscoveryOfType
- (BOOL)startDiscoveryOfType:(smbDeviceType)typeMask
                       added:(void (^)(smbDevice * _Nonnull))deviceAdded
                     removed:(void (^)(smbDevice * _Nonnull))deviceRemoved {
    if (_nameServices) {
        [self stopDiscovery];
    }
    
    _nameServices = netbios_ns_new();
    _addedHandler = deviceAdded;
    _removedHandler = deviceRemoved;
    _typeMask = typeMask;
    
    netbios_ns_discover_callbacks callbacks;
    callbacks.p_opaque = (__bridge void *)(self);
    callbacks.pf_on_entry_added = _on_entry_added;
    callbacks.pf_on_entry_removed = _on_entry_removed;
    /*!Broadcast every 4 seconds
     */
    return (netbios_ns_discover_start(_nameServices,
                                     4,
                                     &callbacks) == 0);
}

#pragma mark - stopDiscovery
- (void)stopDiscovery {
    if (_nameServices) {
        netbios_ns_discover_stop(_nameServices);
        netbios_ns_destroy(_nameServices);
        _nameServices = nil;
        _addedHandler = nil;
        _removedHandler = nil;
    }
}

#pragma mark - _on_entry_added
static void _on_entry_added(void *p_opaque, netbios_ns_entry *entry) {
    if (_addedHandler) {
        dispatch_async(dispatch_get_main_queue(), ^{
            
        });
    }
}

#pragma mark - _on_entry_removed
static void _on_entry_removed(void *p_opaque, netbios_ns_entry *entry) {
    if (_removedHandler) {
        
    }
}

#pragma mark - _device
static smbDevice *_device(netbios_ns_entry *entry) {
    smbDevice *device;
    struct in_addr addr;
    addr.s_addr = netbios_ns_entry_ip(entry);
    
    const char *inet = inet_ntoa(addr);
    const char *group = netbios_ns_entry_group(entry);
    const char *name = netbios_ns_entry_name(entry);
    const char type = netbios_ns_entry_type(entry);
    
    smbDeviceType deviceType = smbDeviceTypeUnknow;
    NSString *ipString =  inet ? [NSString stringWithUTF8String:inet] : nil;
    NSString *groupString = group ? [NSString stringWithUTF8String:group] : nil;
    NSString *nameString = name ? [NSString stringWithUTF8String:name] : nil;
    
    switch (type) {
        case NETBIOS_FILESERVER:
            break;
        case NETBIOS_WORKSTATION:
        case NETBIOS_MESSENGER:
        case NETBIOS_DOMAINMASTER:
        default:
            device = [[smbDevice alloc] initWithType:deviceType
                                                host:ipString
                                         netbiosName:nameString
                                               group:groupString];
            break;
    }
    return device;
}
@end
