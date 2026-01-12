#include <interpreter.h>

void setup_interpret(interpret_t* inter) {
    for (int i = 0; i < LABELS_SIZE; i++)   inter->labels[i] = -1;
    for (int i = 0; i < LINE_SIZE / 4; i++) inter->line[i] = 0;
    inter->flags.cmt  = 0;
    inter->flags.jmp  = 0;
    inter->func.raddr = 0;
}

void interpret(interpret_t* inter) {
    size_t n;
    char buffer[256] = { 0 };
    while ((n = fread(buffer, 1, sizeof(buffer), inter->fp)) > 0) {
        set_tokenizer(buffer, (unsigned int)n, &inter->tokenizer);

        char curr;
        while ((curr = get_next_token(&inter->tokenizer))) {
            print_debug(
                "Command=%c, pos=%i\nFlags:\ncmt=%i;\njmp=%i;\n", 
                curr, inter->tokenizer.pos, inter->flags.cmt, inter->flags.jmp
            );
            debug_wait();

            if (inter->flags.jmp) {
                if (curr - 'a' == inter->flags.jmp) inter->flags.jmp = 0;
                goto _move_next_with_label_reg;
            }

            if (inter->flags.cmt) {
                if (curr == COMMENT_CHAR) inter->flags.cmt = 0;
                continue;
            }

            switch (curr) {
                /* Basic commands */
                case MOVEL_CARRIAGE_CHAR: inter->pos--;                            continue;
                case MOVER_CARRIAGE_CHAR: inter->pos++;                            continue;
                case STDOUT_CHAR: inter->io.putc(inter->line[inter->pos]);         continue;
                case STDIN_CHAR: inter->line[inter->pos] = (char)inter->io.getc(); continue;
                case INCREMENT_CHAR: inter->line[inter->pos]++;                    continue; 
                case DECREMENT_CHAR: inter->line[inter->pos]--;                    continue;

                /* Function setup */
                case TERMINATE_CHAR: {
                    if (inter->func.raddr) {
                        inter->tokenizer.pos = inter->func.raddr;
                        inter->func.raddr = 0;
                        continue;
                    }

                    goto _terminate;
                }

                case FARG_CHAR: inter->func.farg = inter->line[inter->pos]; continue;
                case SARG_CHAR: inter->func.sarg = inter->line[inter->pos]; continue;
                case TARG_CHAR: inter->func.targ = inter->line[inter->pos]; continue;
                case CALL_CHAR: {
                    curr = get_next_token(&inter->tokenizer);
                    inter->func.raddr = inter->tokenizer.pos;
                    inter->tokenizer.pos = inter->labels[curr - 'a'];
                    continue;
                }

                case JUMP_CHAR: {
                    /* If the label already exists,
                       jump to this location */
                    curr = get_next_token(&inter->tokenizer);
                    if (inter->labels[curr - 'a'] >= 0) {
                        print_debug("Jump character was found. Target is the '%c' label [%i]\n", curr, inter->labels[curr - 'a']);
                        inter->tokenizer.pos = inter->labels[curr - 'a'];
                        continue;
                    }

                    print_debug("Jump character '%c' isn't registered in the table. Trying to find...\n", curr);
                    inter->flags.jmp = curr - 'a';
                    continue;
                }

                /* Skip a comment in the source
                   code */
                case COMMENT_CHAR: {
                    inter->flags.cmt = !inter->flags.cmt;
                    continue;
                }

                default: break;
            }
_move_next_with_label_reg: {}
            if (curr >= 'a' && curr <= 'z') {
                print_debug("Registered label='%c' for a position at='%i'\n", curr, inter->tokenizer.pos);
                inter->labels[curr - 'a'] = inter->tokenizer.pos;
            }
        }
    }

_terminate: {}
}
