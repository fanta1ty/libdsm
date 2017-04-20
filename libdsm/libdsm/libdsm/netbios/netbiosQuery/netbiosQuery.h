//
//  netbiosQuery.h
//  test
//
//  Created by Thinh Nguyen Duc on 4/13/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import "netbios_defs.h"

typedef struct netbios_query_s {
    size_t                      payload_size;
    size_t                      cursor;
    netbios_query_packet        *packet;
} netbios_query;

@interface netbiosQuery : NSObject
#pragma mark - netbiosQueryNew
netbios_query *netbios_query_new(size_t payload_size, int is_query, char opcode);

#pragma mark - netbiosQuerySetFlag
void netbios_query_set_flag(netbios_query *q, uint16_t flag, int value);

#pragma mark - netbios_query_append
int netbios_query_append(netbios_query *q, const char *data, size_t data_size);
@end
#endif
