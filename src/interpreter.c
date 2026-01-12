#include <interpreter.h>

void setup_interpret(interpret_t* inter) {
    for (int i = 0; i < LABELS_SIZE; i++)   inter->labels[i] = -1;
    for (int i = 0; i < LINE_SIZE / 4; i++) inter->line[i] = 0;
    inter->flags.cmt = 0;
    inter->flags.jmp = 0;
}

void interpret(interpret_t* inter) {
    size_t n;
    token_line_t tkn;
    char buffer[256] = { 0 };
    while ((n = fread(buffer, 1, sizeof(buffer), inter->fp)) > 0) {
        set_tokenizer(buffer, n, &tkn);

        char curr;
        while ((curr = get_next_token(&tkn))) {
            switch (curr) {
                case TERMINATE_CHAR:                                                    goto _terminate;
                case MOVEL_CARRIAGE_CHAR: inter->line_pos--;                            goto _move_next;
                case MOVER_CARRIAGE_CHAR: inter->line_pos++;                            goto _move_next;
                case STDOUT_CHAR: inter->io.putc(inter->line[inter->line_pos]);         goto _move_next;
                case STDIN_CHAR: inter->line[inter->line_pos] = (char)inter->io.getc(); goto _move_next;
                case INCREMENT_CHAR: inter->line[inter->line_pos]++;                    goto _move_next; 
                case DECREMENT_CHAR: inter->line[inter->line_pos]--;                    goto _move_next;

                case JUMP_CHAR: {
                    /* If the label already exists,
                       jump to this location */
                    curr = get_next_token(&tkn);
                    if (inter->labels[curr - 'A'] >= 0) {
                        inter->line_pos = inter->labels[curr - 'A'];
                        goto _move_next;
                    }

                    inter->flags.jmp = 1;
                    break;
                }

                /* Skip a comment in the source
                   code */
                case COMMENT_CHAR: {
                    inter->flags.cmt = !inter->flags.cmt;
                    goto _move_next;
                }

                default: break;
            }

            inter->labels[curr - 'A'] = inter->line_pos;
_move_next: {}
        }
    }

_terminate: {}
}