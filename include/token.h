#ifndef TOKEN_H_
#define TOKEN_H_

#include <debug.h>

/* base */
#define INCREMENT_CHAR      '+'
#define DECREMENT_CHAR      '-'
#define MOVER_CARRIAGE_CHAR '>'
#define MOVEL_CARRIAGE_CHAR '<'
#define LOOP_INIT_CHAR      '['
#define LOOP_CLOSE_CHAR     ']'
#define STDIN_CHAR          ','
#define STDOUT_CHAR         '.'

/* ++ */
#define COMMENT_CHAR        '/'
#define TERMINATE_CHAR      '!'
#define RESET_CELL          '*'
#define JUMP_CHAR           '^'

#define CALL_CHAR           '&'
#define GET_ARG             '#'
#define GET_RET             '$'

#define DEBUG_CHAR          '?'

typedef struct {
    char*         buffer;
    unsigned long buffer_size;
    long          pos;
} token_line_t;

void set_tokenizer(char* buffer, unsigned long bsize, token_line_t* tl);
char get_next_token(token_line_t* tl);

#endif
