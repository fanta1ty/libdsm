//
//  smbDevice.m
//  libdsm
//
//  Created by trekvn on 4/3/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import "smbDevice.h"

@implementation smbDevice

- (instancetype)initWithType:(smbDeviceType)type
                        host:(NSString *)ipAddressOrHostname
                 netbiosName:(NSString *)netbiosName
                       group:(NSString *)group {
    self = [super init];
    if (self) {
        _type = type;
        _host = ipAddressOrHostname;
        _netbiosName = netbiosName;
        _group = group;
    }
    return self;
}

- (NSString *)description {
    return [NSString stringWithFormat:@"%@ %@(%@)/%@", [self typeName], _netbiosName, _host, _group];
}

- (BOOL)isEqual:(id)object {
    if (object == self) {
        return YES;
    }
    if (object == nil || ![object isKindOfClass:[self class]]) {
        return NO;
    }
    return [self.description isEqualToString:((smbDevice *)object).description];
}

- (NSString *)typeName {
    switch (_type) {
        case smbDeviceTypeWorkstation: {
            return @"Workstation";
            break;
        }
            
        case smbDeviceTypeMessenger: {
            return @"Messenger";
            break;
        }
            
        case smbDeviceTypeDomainMaster: {
            return @"Domain Master";
            break;
        }
            
        case smbDeviceTypeFileServer: {
            return @"File Server";
            break;
        }
            
        default: {
            return @"Unknown Device";
            break;
        }
    }
}
@end
