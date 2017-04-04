//
//  bdsm_debug.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#ifndef bdsm_debug_h
#   define bdsm_debug_h
#   ifdef __ANDROID__
#       define LOG_TAG "libdsm"
#       include <android/log.h>
#       include <stdio.h>
#       include <errno.h>
#       include <stdarg.h>
static inline void bdsm_perror(const char *format, ...) {
    va_list ap;
    char *buf;
    
    va_start(ap, format);
    if (vasprintf(&buf, format, ap) == -1) {
        va_end(ap);
        return;
    }
    
    va_end(ap);
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s%s", buf, strerror(errno));
    
    free(buf);
}
#   else
#       include <stdio.h>
#       define bdsm_perror(...) perror(__VA_ARGS__)
#   endif

#   ifndef NDEBUG
#       ifdef __ANDROID__
#           define bdsm_dbg(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#       else
#           define bdsm_dbg(...) fprintf(stderr, __VA_ARGS__)
#       endif
#   else
#       define bdsm_dbg(...) do { } while(0)
#   endif
#endif /* bdsm_debug_h */
