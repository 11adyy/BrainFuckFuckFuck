#include <interpreter.h>

void setup_interpret(interpret_t* inter) {
    for (int i = 0; i < LABELS_SIZE; i++) inter->labels[i] = -1;
    short stack_pos = 0;
    unsigned int stack[LINE_SIZE] = { 0 };
    for (unsigned int i = 0; i < inter->code_size; i++) {
        switch (inter->code[i]) {
            case LOOP_INIT_CHAR: {
                stack[stack_pos++] = i;
                break;
            }
            case LOOP_CLOSE_CHAR: {
                if (stack_pos > 0) {
                    inter->brackets[i] = stack[--stack_pos];
                    inter->brackets[stack[stack_pos]] = i;
                }

                break;
            }
        }
    }
}

void interpret(interpret_t* inter) {
    char curr;
    set_tokenizer(inter->code, inter->code_size, &inter->tokenizer);
    while ((curr = get_next_token(&inter->tokenizer)) >= 0) {
        if (curr == COMMENT_CHAR) {
            inter->flags.cmt = !inter->flags.cmt;
            continue;
        }

        if (inter->flags.cmt) continue;
        if (inter->flags.jmp) {
            if (curr - 'a' == inter->flags.jmp) inter->flags.jmp = 0;
            goto _move_next_with_label_reg;
        }
        
        func_t* cfunc = &inter->funcs[inter->func_scope];
        switch (curr) {
            case MOVEL_CARRIAGE_CHAR: cfunc->pos--;                            continue;
            case MOVER_CARRIAGE_CHAR: cfunc->pos++;                            continue;
            case STDOUT_CHAR: inter->io.putc(cfunc->line[cfunc->pos]);         continue;
            case STDIN_CHAR: cfunc->line[cfunc->pos] = (char)inter->io.getc(); continue;
            case INCREMENT_CHAR: cfunc->line[cfunc->pos]++;                    continue; 
            case DECREMENT_CHAR: cfunc->line[cfunc->pos]--;                    continue;
            case RESET_CELL: cfunc->line[cfunc->pos] = 0;                      continue;
            case GET_RET: cfunc->line[cfunc->pos] = cfunc->ret;                continue;
            case LOOP_INIT_CHAR: {
                if (!cfunc->line[cfunc->pos]) inter->tokenizer.pos = inter->brackets[inter->tokenizer.pos];
                continue;
            }
            case LOOP_CLOSE_CHAR: {
                if (cfunc->line[cfunc->pos]) inter->tokenizer.pos = inter->brackets[inter->tokenizer.pos];
                continue;
            }
            case TERMINATE_CHAR: {
                if (!inter->func_scope) return;
                inter->funcs[inter->func_scope - 1].ret = cfunc->line[cfunc->pos];
                inter->tokenizer.pos = inter->funcs[inter->func_scope - 1].raddr;
                inter->funcs[--inter->func_scope].raddr = 0;
                continue;
            }
            case JUMP_CHAR: {
                curr = get_next_token(&inter->tokenizer);
                if (inter->labels[curr - 'a'] >= 0) {
                    inter->tokenizer.pos = inter->labels[curr - 'a'];
                    continue;
                }

                inter->flags.jmp = curr - 'a';
                continue;
            }
            case CALL_CHAR: {
                curr = get_next_token(&inter->tokenizer);
                inter->funcs[inter->func_scope++].raddr = inter->tokenizer.pos;
                inter->tokenizer.pos = inter->labels[curr - 'a'];
                continue;
            }
            case GET_ARG: {
                curr = get_next_token(&inter->tokenizer);
                if (inter->func_scope <= 0) continue;
                if (curr >= '0' && curr <= '9') cfunc->line[cfunc->pos] = inter->funcs[inter->func_scope - 1].args[curr - '0'];
                continue;
            }
            default: break;
        }
        
        if (curr >= '0' && curr <= '9') inter->funcs[inter->func_scope].args[curr - '0'] = cfunc->line[cfunc->pos];
_move_next_with_label_reg: {}
        if (
            curr >= 'a' && curr <= 'z' && 
            inter->labels[curr - 'a'] < 0
        ) inter->labels[curr - 'a'] = inter->tokenizer.pos;
    }
}
