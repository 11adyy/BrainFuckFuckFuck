#include <interpreter.h>

void setup_interpret(interpret_t* inter) {
    for (int i = 0; i < LABELS_SIZE; i++)   inter->labels[i] = -1;
    for (int i = 0; i < LINE_SIZE / 4; i++) ((int*)(inter->line))[i] = 0;
    for (int i = 0; i < LINE_SIZE / 4; i++) ((int*)(inter->code))[i] = 0;
    for (int i = 0; i < LINE_SIZE; i++)     inter->brackets[i] = 0;
    inter->flags.cmt  = 0;
    inter->flags.jmp  = 0;
    inter->func.raddr = 0;
}

void interpret(interpret_t* inter) {
    size_t n;
    if ((n = fread(inter->code, 1, sizeof(inter->code), inter->fp)) <= 0) return;
    set_tokenizer(inter->code, (unsigned short)n, &inter->tokenizer);

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
            case MOVEL_CARRIAGE_CHAR: inter->pos--;                            continue;
            case MOVER_CARRIAGE_CHAR: inter->pos++;                            continue;
            case STDOUT_CHAR: inter->io.putc(inter->line[inter->pos]);         continue;
            case STDIN_CHAR: inter->line[inter->pos] = (char)inter->io.getc(); continue;
            case INCREMENT_CHAR: inter->line[inter->pos]++;                    continue; 
            case DECREMENT_CHAR: inter->line[inter->pos]--;                    continue;
            case TERMINATE_CHAR: {
                if (inter->func.raddr) {
                    inter->tokenizer.pos = inter->func.raddr;
                    inter->func.raddr = 0;
                    continue;
                }

                goto _terminate;
            }
            case CALL_CHAR: {
                curr = get_next_token(&inter->tokenizer);
                inter->func.raddr = inter->tokenizer.pos;
                inter->tokenizer.pos = inter->labels[curr - 'a'];
                continue;
            }
            case IF_CHAR: {
                if (inter->line[inter->pos]) goto _force_jump;
                else {
                    get_next_token(&inter->tokenizer);
                    goto _force_jump;
                }
            }
            case JUMP_CHAR: {
_force_jump: {}
                curr = get_next_token(&inter->tokenizer);
                if (inter->labels[curr - 'a'] >= 0) {
                    inter->tokenizer.pos = inter->labels[curr - 'a'];
                    continue;
                }

                inter->flags.jmp = curr - 'a';
                continue;
            }
            case COMMENT_CHAR: {
                inter->flags.cmt = !inter->flags.cmt;
                continue;
            }
            default: break;
        }

        if (curr >= '0' && curr <= '9') {
            print_debug("Registered argument='%c' for a position at='%i'\n", curr - '0', inter->line[inter->pos]);
            inter->func.args[curr - '0'] = inter->line[inter->pos];
        }
_move_next_with_label_reg: {}
        
        if (curr >= 'a' && curr <= 'z') {
            print_debug("Registered label='%c' for a position at='%i'\n", curr, inter->tokenizer.pos);
            inter->labels[curr - 'a'] = inter->tokenizer.pos;
        }
    }
_terminate: {}
}
