//
//  smb_transport.h
//  test
//
//  Created by trekvn on 4/10/17.
//  Copyright © 2017 trekvn. All rights reserved.
//


#ifndef smb_transport_h
#define smb_transport_h

#include "smb_types.h"
#include <stdio.h>

#pragma mark - smb_transport_nbt

/*!Fill the smb_transport structure with the fun pointers for using NBT transport
 */
void smb_transport_nbt(smb_transport *tr);

#pragma mark - smb_transport_tcp
/*!Fill the smb_transport structure with the fun pointers for using DirectTCP transport
 */
void smb_transport_tcp(smb_transport *tr);


#endif /* smb_transport_h */
