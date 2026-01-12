#ifndef TOKEN_H_
#define TOKEN_H_

#include <debug.h>

typedef struct {
    char*          buffer;
    unsigned short buffer_size;
    int            pos;
} token_line_t;

void set_tokenizer(char* buffer, unsigned short bsize, token_line_t* tl);
char get_next_token(token_line_t* tl);

#endif
