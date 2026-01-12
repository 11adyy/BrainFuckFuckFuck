#ifndef HARD_H_
#define HARD_H_

#include <stdio.h>

typedef struct {
    int (*putc)(int);
    int (*getc)();
} io_t;

#endif
