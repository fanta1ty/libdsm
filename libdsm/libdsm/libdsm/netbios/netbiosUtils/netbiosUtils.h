//
//  netbiosUtils.h
//  test
//
//  Created by Thinh Nguyen Duc on 4/13/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import "netbios_defs.h"

@interface netbiosUtils : NSObject
#pragma mark - netbiosNameLevel1Encode
void  netbios_name_level1_encode(const char *name, char *encoded_name,
                                 unsigned type);

#pragma mark - netbiosNameLevel1Decode
void  netbios_name_level1_decode(const char *encoded_name, char *name);

#pragma mark - netbiosNameEncode
char *netbios_name_encode(const char *name, char *domain,
                           unsigned type);

#pragma mark - netbiosNameDecode
int netbios_name_decode(const char *encoded_name,
                          char *name, char **domain);
@end
#endif
