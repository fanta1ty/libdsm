//
//  smbDevice.h
//  libdsm
//
//  Created by trekvn on 4/3/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_OPTIONS(NSUInteger, smbDeviceType) {
    smbDeviceTypneNone = 0,
    
    smbDeviceTypeUnknow = 1 << 0,
    smbDeviceTypeWorkstation = 1 << 1,
    smbDeviceTypeMessenger = 1 << 2,
    smbDeviceTypeFileServer = 1 << 3,
    smbDeviceTypeDomainMaster = 1 << 4,
    
    smbDeviceTypeAny = smbDeviceTypeUnknow | smbDeviceTypeWorkstation | smbDeviceTypeMessenger | smbDeviceTypeFileServer | smbDeviceTypeDomainMaster
};

@interface smbDevice : NSObject
@property (readonly, nonatomic) smbDeviceType               type;
@property (readonly, nonatomic) NSString                    *host;
@property (readonly, nonatomic) NSString                    *netbiosName;
@property (readonly, nonatomic) NSString                    *group;

- (instancetype)initWithType:(smbDeviceType)type
                        host:(NSString *)ipAddressOrHostname
                 netbiosName:(NSString *)netbiosName
                       group:(NSString *)group;


@end
