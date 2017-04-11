//
//  smb_spnego.h
//  test
//
//  Created by trekvn on 4/11/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef smb_spnego_h
#define smb_spnego_h

#include <stdio.h>
#include "smb_types.h"

int             smb_session_login_spnego(smb_session *s, const char *domain,
                                         const char *user, const char *password);
#endif /* smb_spnego_h */
