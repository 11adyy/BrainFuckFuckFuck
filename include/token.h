#ifndef TOKEN_H_
#define TOKEN_H_

typedef struct {
    char* buffer;
    int   buffer_size;
    int   pos;
} token_line_t;

void set_tokenizer(char* buffer, unsigned int bsize, token_line_t* tl);
char get_next_token(token_line_t* tl);

#endif
