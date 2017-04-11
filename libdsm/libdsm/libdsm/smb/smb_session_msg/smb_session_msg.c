//
//  smb_session_msg.c
//  test
//
//  Created by trekvn on 4/10/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#include "smb_session_msg.h"
#include "config.h"

#include <assert.h>

#include "smb_session.h"
#include "smb_message.h"


int             smb_session_send_msg(smb_session *s, smb_message *msg)
{
    size_t        pkt_sz;
    
    assert(s != NULL);
    assert(s->transport.session != NULL);
    assert(msg != NULL && msg->packet != NULL);
    
    msg->packet->header.flags   = 0x18;
    msg->packet->header.flags2  = 0xc843;
    // msg->packet->header.flags2  = 0xc043; // w/o extended security;
    msg->packet->header.uid = s->srv.uid;
    
    s->transport.pkt_init(s->transport.session);
    
    pkt_sz = sizeof(smb_packet) + msg->cursor;
    if (!s->transport.pkt_append(s->transport.session, (void *)msg->packet, pkt_sz))
        return 0;
    if (!s->transport.send(s->transport.session))
        return 0;
    
    return 1;
}

size_t          smb_session_recv_msg(smb_session *s, smb_message *msg)
{
    void                      *data;
    ssize_t                   payload_size;
    
    assert(s != NULL && s->transport.session != NULL);
    
    payload_size = s->transport.recv(s->transport.session, &data);
    if (payload_size <= 0)
        return 0;
    
    if ((size_t)payload_size < sizeof(smb_header))
        return 0;
    
    if (msg != NULL)
    {
        msg->packet = (smb_packet *)data;
        msg->payload_size = payload_size - sizeof(smb_header);
        msg->cursor       = 0;
    }
    
    return payload_size - sizeof(smb_header);
}
