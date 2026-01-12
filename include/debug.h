#ifndef DEBUG_H_
#define DEBUG_H_
#ifdef DEBUG
    #include <stdio.h>
    #include <stdarg.h>
    #define print_debug(msg, ...) fprintf(stdout, msg, __VA_ARGS__)
    #define debug_wait() getc(stdin);
#else
    #define print_debug(msg, ...) {}
    #define debug_wait() {}
#endif
#endif
