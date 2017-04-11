//
//  netbios_utils.h
//  test
//
//  Created by trekvn on 4/10/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef netbios_utils_h
#define netbios_utils_h

#include "netbios_defs.h"
#include <stdio.h>

void  netbios_name_level1_encode(const char *name, char *encoded_name,
                                 unsigned type);
void  netbios_name_level1_decode(const char *encoded_name, char *name);

// XXX: domain support is not implemented
char  *netbios_name_encode(const char *name, char *domain,
                           unsigned type);
int   netbios_name_decode(const char *encoded_name,
                          char *name, char **domain);
#endif /* netbios_utils_h */
