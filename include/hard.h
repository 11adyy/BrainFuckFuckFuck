#ifndef HARD_H_
#define HARD_H_

#include <stdio.h>

#define LINE_SIZE   10000
#define LABELS_SIZE 25

typedef struct {
    int (*putc)(int);
    int (*getc)();
} io_t;

#endif
