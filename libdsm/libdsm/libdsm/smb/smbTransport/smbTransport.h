//
//  smbTransport.h
//  test
//
//  Created by trekvn on 4/13/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import "smbHeader.h"

@interface smbTransport : NSObject
#pragma mark - smbTransportNBT
/*!Fill the smb_transport structure with the fun pointers for using NBT transport
 */
void smb_transport_nbt(smb_transport *tr);

#pragma mark - smb_transport_tcp
/*!Fill the smb_transport structure with the fun pointers for using DirectTCP transport
 */
void smb_transport_tcp(smb_transport *tr);
@end
#endif
