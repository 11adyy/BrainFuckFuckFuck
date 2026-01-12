#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <hard.h>
#include <dict.h>
#include <debug.h>
#include <token.h>

#define LINE_SIZE   10000
#define LABELS_SIZE 25

typedef struct {
    io_t         io;
    token_line_t tokenizer;

    int          pos;
    char         line[LINE_SIZE];
    char         code[LINE_SIZE];
    int          brackets[LINE_SIZE];
    int          labels[LABELS_SIZE];
    FILE*        fp;

    struct {
        int      raddr; /* Function's return address  */
        int      args[10];
    } func;

    struct {
        char     jmp;
        char     cmt;
    } flags;
} interpret_t;

void setup_interpret(interpret_t* inter);
void interpret(interpret_t* inter);

#endif
