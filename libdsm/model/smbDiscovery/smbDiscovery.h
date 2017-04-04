//
//  smbDiscovery.h
//  libdsm
//
//  Created by trekvn on 4/3/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "smbDevice.h"

@interface smbDiscovery : NSObject
+ (nullable instancetype)sharedInstance;

- (BOOL)startDiscoveryOfType:(smbDeviceType)typeMask
                       added:(nullable void(^)(smbDevice *_Nonnull device))deviceAdded
                     removed:(nullable void(^)(smbDevice *_Nonnull device))deviceRemoved;

- (void)stopDiscovery;
@end
