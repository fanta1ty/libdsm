//
//  compat.h
//  libdsm
//
//  Created by trekvn on 4/3/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <stdlib.h>
#if !defined HAVE_CLOCK_GETTIME
int clock_gettime(clockid_t clk_id, struct timespec *tp);
#endif

#if !defined HAVE_STRNDUP
char *strndup(const char *str, size_t n);
#endif

#ifndef O_NONBLOCK
# define O_NONBLOCK 0
#endif

#if !defined HAVE_SYS_QUEUE_H
# include "queue.h"
#endif

#if !defined(HAVE_PIPE) && defined(HAVE__PIPE)
#define HAVE_PIPE
int pipe(int fds[2]);
#endif

#ifndef _WIN32
#define closesocket(fd) close(fd)
#endif
