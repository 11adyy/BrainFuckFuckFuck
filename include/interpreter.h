#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <stdio.h>
#include <debug.h>
#include <token.h>

#define LINE_SIZE   10000
#define LABELS_SIZE 25
#define SCOPE_STACK 10

typedef struct {
    int (*putc)(int);
    int (*getc)();
} io_t;

typedef struct {
    long raddr;   /* Function's return address */
    int args[10]; /* Function's 10 arguments   */
} func_t;

typedef struct {
    io_t          io; 
    token_line_t  tokenizer; 
    /* Data tape information */
    int           pos;
    int           line[LINE_SIZE];
    /* Source code information */
    unsigned long code_size;
    char          code[LINE_SIZE];
    /* Brackets & Labels information */
    unsigned int  brackets[LINE_SIZE];
    long          labels[LABELS_SIZE];
    /* Functions information */
    int           func_scope;
    func_t        funcs[SCOPE_STACK];
    /* Flags information */
    struct {
        char      jmp;
        char      cmt;
    } flags;
} interpret_t;

void setup_interpret(interpret_t* inter);
void interpret(interpret_t* inter);

#endif
