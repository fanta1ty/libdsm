//
//  compat.c
//  test
//
//  Created by trekvn on 4/7/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#include "compat.h"
#include "config.h"

#if !defined(HAVE_PIPE) && defined(HAVE__PIPE)

#include <fcntl.h>

int pipe(int fds[2]) {
    return _pipe(fds, 32768, O_NOINHERIT | O_BINARY);
}
#endif

#if !defined HAVE_CLOCK_GETTIME
#   ifndef _WIN32
#       include <sys/time.h>
#       include <sys/resource.h>
#       include <mach/mach.h>
#       include <mach/clock.h>
#       include <mach/mach_time.h>
#       include <unistd.h>
#       include <sched.h>
#       include "compat.h"

int clock_gettime(clockid_t clk_id, struct timespec *tp) {
    kern_return_t ret;
    clock_serv_t clk;
    clock_id_t clk_serv_id;
    mach_timespec_t tm;
    
    if (clk_id != CLOCK_REALTIME) {
        return -1;
    }
    
    clk_serv_id = clk_id == CLOCK_REALTIME ? CALENDAR_CLOCK : SYSTEM_CLOCK;
    
    if (KERN_SUCCESS == (ret = host_get_clock_service(mach_host_self(), clk_serv_id, &clk))) {
        if (KERN_SUCCESS == (ret = clock_get_time(clk, &tm))) {
            tp->tv_sec  = tm.tv_sec;
            tp->tv_nsec = tm.tv_nsec;
        }
    }
    if (KERN_SUCCESS != ret) {
        return -1;
    }
    
    return 0;
}

#   else
#       include <windows.h>
#       include <stdint.h>
#       include <time.h>
#       include "compat.h"

int clock_gettime(int clk_id, struct timespec *spec) {
    (void)clk_id;
    FILETIME wintime;
    GetSystemTimeAsFileTime(&wintime);
    uint64_t t = (uint64_t)wintime.dwHighDateTime << 32 | wintime.dwLowDateTime;
    t -= 116444736000000000;
    spec->tv_sec  = t / 10000000;
    spec->tv_nsec = t % 10000000 * 100;
    return 0;
}
#   endif
#endif

