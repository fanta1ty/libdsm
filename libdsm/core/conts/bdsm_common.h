//
//  bdsm_common.h
//  libdsm
//
//  Created by trekvn on 4/3/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifdef _MSC_VER
#   if defined(__clang__)
#       define smb_packed_start
#       define smb_packed_end       __attribute__((packed))
#   else
#       define smb_packed_start     __pragma(pack(push, 1))
#       define smb_packed_end       __pragma(pack(pop))
#endif
#elif defined(__GNUC__)
#   define smb_packed_start
#   ifdef _WIN32
#       define smb_packed_end       __attribute__((packed, gcc_struct))
#   else
#       define smb_packed_end       __attribute__((packed))
#   endif
#endif
