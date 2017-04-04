//
//  bdsm_common.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef bdsm_common_h
#define bdsm_common_h

#ifdef _MSC_VER
#   if defined(__clang__)
#       define SMB_PACKED_START
#       define SMB_PACKED_END    __attribute__((packed))
#   else
#       define SMB_PACKED_START  __pragma(pack(push, 1))
#       define SMB_PACKED_END    __pragma(pack(pop))
#endif
#elif defined(__GNUC__)
#   define SMB_PACKED_START
#   ifdef _WIN32
#       define SMB_PACKED_END    __attribute__((packed, gcc_struct))
#   else
#       define SMB_PACKED_END    __attribute__((packed))
#   endif
#endif

#endif /* bdsm_common_h */
