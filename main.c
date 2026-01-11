#include <dict.h>
#include <hard.h>

int _line_pos             = 0;
char _line[LINE_SIZE]     = { 0 };
char _labels[LABELS_SIZE] = { -1 };
io_t io                   = { 0 };

static int _interpret(FILE* fp) {
    char buffer[256] = { 0 };

    int foffset = 0;
    ssize_t bytes_read = 0;
    while ((bytes_read = fread(buffer, sizeof(buffer), 1, fp))) {
        char* h = (char*)buffer;
        while(*h) {
            /* Main logic handler in
               the interpreter */
            switch (*h) {
                case TERMINATE_CHAR:                           goto _terminate;
                case MOVEL_CARRIAGE_CHAR: _line_pos--;         goto _move_next;
                case MOVER_CARRIAGE_CHAR: _line_pos++;         goto _move_next;
                case STDOUT_CHAR: io.putc(_line[_line_pos]);   goto _move_next;
                case STDIN_CHAR: _line[_line_pos] = io.getc(); goto _move_next;

                case JUMP_CHAR: {
                    /* If the label already exists,
                       jump to this location */
                    if (_labels[*h - 'A'] >= 0) {
                        _line_pos = _labels[*h - 'A'];
                        goto _move_next;
                    }
                    /* Otherwise, the lable exists somewhere
                       further. Wait until we're able to locate it */
                    else {
                        char l = *h++;
                        while (*h && *h != l) h++;
                        break;
                    }
                }

                default: break;
            }

            /* Register a new label in 
               the label table */
            _labels[*h - 'A'] = _line_pos;
_move_next: {}
            h++;
        }
    }

_terminate: {}
    return 1;
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
    
    _interpret(fp);

    fclose(fp);
    return 0;
}
