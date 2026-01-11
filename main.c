#include <dict.h>
#include <hard.h>

int _line_pos            = 0;
char _line[LINE_SIZE]    = { 0 };
int _labels[LABELS_SIZE] = { -1 };
io_t io                  = { 0 };

static int _interpret(FILE* fp) {
    size_t n;
    char buffer[256] = { 0 };
    while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        for (size_t i = 0; i < n; ++i) {
            char c = buffer[i];
            /* Main logic handler in
               the interpreter */
            switch (c) {
                case TERMINATE_CHAR:                                 goto _terminate;
                case MOVEL_CARRIAGE_CHAR: _line_pos--;               goto _move_next;
                case MOVER_CARRIAGE_CHAR: _line_pos++;               goto _move_next;
                case STDOUT_CHAR: io.putc(_line[_line_pos]);         goto _move_next;
                case STDIN_CHAR: _line[_line_pos] = (char)io.getc(); goto _move_next;
                case INCREMENT_CHAR: _line[_line_pos]++;             goto _move_next; 
                case DECREMENT_CHAR: _line[_line_pos]--;             goto _move_next;

                case JUMP_CHAR: {
                    /* If the label already exists,
                       jump to this location */
                    c = buffer[++i];
                    if (_labels[c - 'A'] >= 0) {
                        _line_pos = _labels[c - 'A'];
                        goto _move_next;
                    }
                    /* Otherwise, the lable exists somewhere
                       further. Wait until we're able to locate it */
                    char l = c;
                    while (buffer[++i] != l);
                    break;
                }

                /* Skip a comment in the source
                   code */
                case COMMENT_CHAR: {
                    while (buffer[++i] != COMMENT_CHAR);
                    goto _move_next;
                }

                default: break;
            }

            /* Register a new label in 
               the label table */
            _labels[c - 'A'] = _line_pos;
_move_next: {}
        }
    }

_terminate: {}
    return 1;
}

static int _putc(int c) {
    return putc(c, stdout);
}

static int _getc() {
    return getc(stdin);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "You must provide the target file: ./%s <path>\n", argv[0]);
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "File '%s' isn't found!\n", argv[1]);
        return 1;
    }
    
    io = (io_t){
        .getc = _getc,
        .putc = _putc
    };

    _interpret(fp);

    fclose(fp);
    return 0;
}
