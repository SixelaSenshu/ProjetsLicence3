#ifndef _MACROS_H_
#define _MACROS_H_
#include <stdio.h>
#include <stdlib.h>

#define ERROR_EXIT(msg,state)                                  \
    perror(msg);                                               \
    __assert_fail(msg,__FILE__,__LINE__,__ASSERT_FUNCTION) ;   \
    exit(state);                                               \

#endif                       