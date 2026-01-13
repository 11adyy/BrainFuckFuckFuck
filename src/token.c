#include <token.h>

void set_tokenizer(char* buffer, unsigned long bsize, token_line_t* tl) {
    tl->buffer = buffer;
    tl->buffer_size = bsize;
    tl->pos = -1;
}

char get_next_token(token_line_t* tl) {
    if (++tl->pos >= (long)tl->buffer_size) return -1;
    return tl->buffer[tl->pos];
}
