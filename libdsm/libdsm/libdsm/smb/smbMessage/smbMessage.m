//
//  smbMessage.m
//  test
//
//  Created by trekvn on 4/14/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import "config.h"
#import "smbMessage.h"

#define PAYLOAD_BLOCK_SIZE 256

@implementation smbMessage

static int smb_message_expand_payload(smb_message *msg, size_t cursor, size_t data_size)
{
    if (data_size == 0 || data_size > msg->payload_size - cursor)
    {
        size_t new_size = data_size + cursor - msg->payload_size;
        size_t nb_blocks = (new_size / PAYLOAD_BLOCK_SIZE) + 1;
        size_t new_payload_size = msg->payload_size + nb_blocks * PAYLOAD_BLOCK_SIZE;
        void *new_packet = realloc(msg->packet, sizeof(smb_packet) + new_payload_size);
        if (!new_packet)
            return 0;
        msg->packet = new_packet;
        msg->payload_size = new_payload_size;
    }
    return 1;
}

#pragma mark - smbMessageNew
smb_message   *smb_message_new(uint8_t cmd)
{
    const uint8_t magic[4] = SMB_MAGIC;
    smb_message *msg;
    
    msg = (smb_message *)calloc(1, sizeof(smb_message));
    if (!msg)
        return NULL;
    
    if (smb_message_expand_payload(msg, msg->cursor, 0) == 0) {
        free(msg);
        return NULL;
    }
    memset(msg->packet, 0, sizeof(smb_packet));
    
    for (unsigned i = 0; i < 4; i++)
        msg->packet->header.magic[i] = magic[i];
    msg->packet->header.command   = cmd;
    msg->packet->header.pid       = getpid();
    
    return msg;
}

#pragma mark - smbMessageGrow
smb_message *smb_message_grow(smb_message *msg, size_t size)
{
    smb_message *copy;
    
    if (msg == NULL || msg->packet == NULL)
        return NULL;
    
    copy = malloc(sizeof(smb_message));
    if (!copy)
        return NULL;
    copy->cursor        = msg->cursor;
    copy->payload_size  = msg->payload_size + size;
    
    copy->packet = malloc(sizeof(smb_packet) + copy->payload_size);
    if (!copy->packet) {
        free(copy);
        return NULL;
    }
    memcpy((void *)copy->packet, (void *)msg->packet,
           msg->payload_size + sizeof(smb_packet));
    
    return copy;
}

#pragma mark - smbMessageDestroy
void smb_message_destroy(smb_message *msg)
{
    if (msg == NULL)
        return;
    free(msg->packet);
    free(msg);
}
#pragma mark - smbMessageAdvance
int smb_message_advance(smb_message *msg, size_t size)
{
    if (msg == NULL)
        return -1;
    
    if (smb_message_expand_payload(msg, msg->cursor, size) == 0)
        return 0;
    
    msg->cursor += size;
    return 1;
}

#pragma mark - smbMessageAppend
int smb_message_append(smb_message *msg, const void *data,
                                   size_t data_size)
{
    if (msg == NULL || data == NULL)
        return -1;
    
    if (smb_message_expand_payload(msg, msg->cursor, data_size) == 0)
        return 0;
    
    memcpy(msg->packet->payload + msg->cursor, data, data_size);
    msg->cursor += data_size;
    
    return 1;
}

#pragma mark - smbMessageInsert
int smb_message_insert(smb_message *msg, size_t cursor,
                                   const void *data, size_t data_size)
{
    if (msg == NULL || data == NULL)
        return -1;
    
    if (smb_message_expand_payload(msg, cursor, data_size) == 0)
        return 0;
    
    memcpy(msg->packet->payload + cursor, data, data_size);
    
    return 1;
}

#pragma mark - smbMessagePut8
int smb_message_put8(smb_message *msg, uint8_t data)
{
    return smb_message_append(msg, (void *)&data, 1);
}

#pragma mark - smbMessagePut16
int smb_message_put16(smb_message *msg, uint16_t data)
{
    return smb_message_append(msg, (void *)&data, 2);
}

#pragma mark - smbMessagePut32
int smb_message_put32(smb_message *msg, uint32_t data)
{
    return smb_message_append(msg, (void *)&data, 4);
}

#pragma mark - smbMessagePut64
int smb_message_put64(smb_message *msg, uint64_t data)
{
    return smb_message_append(msg, (void *)&data, 8);
}

#pragma mark - smbMessagePutUtf16
size_t smb_message_put_utf16(smb_message *msg, const char *str,
                                      size_t str_len)
{
    char          *utf_str;
    size_t        utf_str_len;
    int           res;
    
    utf_str_len = smb_to_utf16(str, str_len, &utf_str);
    res = smb_message_append(msg, utf_str, utf_str_len);
    free(utf_str);
        
    if (res)
        return utf_str_len;
    return 0;
}

#pragma mark - smbMessagePutUuid
int smb_message_put_uuid(smb_message *msg, uint32_t a, uint16_t b,
                                     uint16_t c, const uint8_t d[8])
{
    if (msg == NULL)
        return -1;
    
    if (!smb_message_put32(msg, a))
        return 0;
    if (!smb_message_put16(msg, b))
        return 0;
    if (!smb_message_put16(msg, c))
        return 0;
    for (int i = 0; i < 8; i++)
        if (!smb_message_put8(msg, d[i]))
            return 0;
    
    return 1;
}

#pragma mark - smbMessageSetAndxMembers
void smb_message_set_andx_members(smb_message *msg)
{
    // This could have been any type with the 'SMB_ANDX_MEMBERS';
    smb_session_req   *req;
    
    if (msg == NULL)
        return;
    
    req = (smb_session_req *)msg->packet->payload;
    req->andx           = 0xff;
    req->andx_reserved  = 0;
    req->andx_offset    = 0;
}

#pragma mark - smbMessageFlag
void smb_message_flag(smb_message *msg, uint32_t flag, int value)
{
    uint32_t      *flags;
    
    if (msg == NULL || msg->packet == NULL)
        return;
    
    // flags + flags2 is actually 24 bit long, we have to be cautious
    flags = (uint32_t *)&(msg->packet->header.flags);
    flag &= 0x00FFFFFF;
    
    if (value)
        *flags |= flag;
    else
        *flags &= ~flag;
}
@end
