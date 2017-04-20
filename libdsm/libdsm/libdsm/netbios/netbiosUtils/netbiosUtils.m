//
//  netbiosUtils.m
//  test
//
//  Created by Thinh Nguyen Duc on 4/13/17.
//  Copyright © 2017 trekvn. All rights reserved.
//
#import <string.h>
#import <ctype.h>

#import "netbiosUtils.h"
#import "config.h"

@implementation netbiosUtils

static short  nibble_encode(char c) {
    short n1, n2;
    
    n1 = (toupper(c) >> 4)   + 'A';
    n2 = (toupper(c) & 0x0F) + 'A';
    
    return ((n1 << 8) | n2);
}

static char   nibble_decode(char c1, char c2) {
    return (((c1 - 'A') << 4) + (c2 - 'A'));
}

#pragma mark - netbiosNameLevel1Encode
void  netbios_name_level1_encode(const char *name, char *encoded_name,
                                 unsigned type) {
    size_t    name_length = strlen(name);
    
    if (name_length > NETBIOS_NAME_LENGTH)
        name_length = NETBIOS_NAME_LENGTH;
    
    for (unsigned int i = 0; i < NETBIOS_NAME_LENGTH; i++) {
        if (i < name_length)
        {
            encoded_name[2 * i]     = nibble_encode(name[i]) >> 8;
            encoded_name[2 * i + 1] = nibble_encode(name[i]) & 0x00FF;
        }
        else
        {
            encoded_name[2 * i]     = 'C';
            encoded_name[2 * i + 1] = 'A';
        }
    }
    
    encoded_name[30] = nibble_encode(type) >> 8;
    encoded_name[31] = nibble_encode(type) &  0x00FF;
    encoded_name[32] = '\0';
}

#pragma mark - netbiosNameLevel1Decode
void  netbios_name_level1_decode(const char *encoded_name, char *name)
{
    for (unsigned int i = 0; i < NETBIOS_NAME_LENGTH; i++)
    {
        name[i] = nibble_decode(encoded_name[2 * i], encoded_name[2 * i + 1]);
    }
    name[NETBIOS_NAME_LENGTH] = 0;
}

#pragma mark - netbiosNameEncode
char *netbios_name_encode(const char *name, char *domain,
                           unsigned type)
{
    (void)domain;  // Unused yet
    
    size_t    encoded_size = 34; // length byte + 32 bytes for encoded name + terminator
    char      *encoded_name;
    
    if (!name)
        return NULL;
    
    encoded_name = malloc(encoded_size);
    if (!encoded_name)
        return NULL;
    encoded_name[0] = 32; // length of the field;
    netbios_name_level1_encode(name, encoded_name + 1, type);
    encoded_name[33] = 0;
    
    //printf("Encoded name (l2): %s.\n", encoded_name);
    
    return encoded_name;
}

#pragma mark - netbiosNameDecode
int netbios_name_decode(const char *encoded_name,
                                    char *name, char **domain)
{
    (void)domain;  // Unused yet
    
    size_t  encoded_length;
    
    if (!encoded_name || !name)
        return -1;
    
    encoded_length =  strlen(encoded_name);
    
    // XXX Support domain name :p
    if (encoded_length != 33)
        return -1;
    
    netbios_name_level1_decode(encoded_name + 1, name);
    name[33] = '\0';
    return 32;
}
@end
