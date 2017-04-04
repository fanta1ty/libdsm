//
//  smb_utils.c
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#include "smb_utils.h"
#include "bdsm_debug.h"

#include <assert.h>
#include <iconv.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <langinfo.h>

#if HAVE_LANGINFO_H && !defined( __APPLE__ )
#   include <langinfo.h>
#endif

static const char *current_encoding() {
#if defined( __APPLE__ )
    return "UTF8";
#elif !HAVE_LANGINFO_H
    return "UTF-8";
#else
    static int locale_set = 0;
    
    if (!locale_set) {
        setlocale(LC_ALL, "");
        locale_set = 1;
    }
    //BDSM_dbg("%s\n", nl_langinfo(CODESET));
    return nl_langinfo(CODESET);
#endif
}

static size_t smb_iconv(const char *src, size_t src_len, char **dst,
                        const char *src_enc, const char *dst_enc) {
    iconv_t ic;
    size_t ret = 0;
    
    assert(src != NULL && dst != NULL && src_enc != NULL && dst_enc != NULL);
    
    if (!src_len) {
        *dst = NULL;
        return 0;
    }
    
    if ((ic = iconv_open(dst_enc, src_enc)) == (iconv_t)-1) {
        bdsm_dbg("Unable to open iconv to convert from %s to %s\n",
                 src_enc, dst_enc);
        *dst = NULL;
        return 0;
    }
    
    for (unsigned mul = 4; mul < 16; mul++) {
        size_t outlen = mul * src_len;
        char *out = malloc(outlen);
        
        const char *inp = src;
        size_t inb = src_len;
        char *outp = out;
        size_t outb = outlen;
        
        if (!out)
            break;
        if (iconv(ic, (char **)&inp, &inb, &outp, &outb) != (size_t)(-1)) {
            ret = outlen - outb;
            *dst = out;
            break;
        }
        free(out);
        if (errno != E2BIG)
            break;
    }
    iconv_close(ic);
    
    if (ret == 0)
        *dst = NULL;
    return ret;
}

size_t smb_to_utf16(const char *src, size_t src_len, char **dst) {
    return (smb_iconv(src, src_len, dst,
                      current_encoding(), "UCS-2LE"));
}

size_t smb_from_utf16(const char *src, size_t src_len, char **dst) {
    return (smb_iconv(src, src_len, dst,
                      "UCS-2LE", current_encoding()));
}
