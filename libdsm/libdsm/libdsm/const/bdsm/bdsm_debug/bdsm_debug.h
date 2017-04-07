//
//  bdsm_debug.h
//  test
//
//  Created by trekvn on 4/7/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef bdsm_debug_h
#define bdsm_debug_h

#include <stdio.h>
#define bdsm_perror(...) perror(__VA_ARGS__)

#ifndef NDEBUG
#   define bdsm_dbg(...) fprintf(stderr, __VA_ARGS__)
#else
#   define bdsm_dbg(...) do { } while(0)
#endif

#endif /* bdsm_debug_h */
