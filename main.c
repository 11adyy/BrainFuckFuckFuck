#include <interpreter.h>

static int _putc(int c) { return putc(c, stdout); }
static int _getc() { return getc(stdin); }

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
    
    interpret_t inter = {
        .tokenizer = { 0 }, .flags = { 0 }, .funcs = { 0 }, .code = { 0 }, .brackets = { 0 }, .func_scope = 0,
        .io = (io_t){
            .getc = _getc,
            .putc = _putc
        },
    };

    if ((inter.code_size = fread(inter.code, 1, sizeof(inter.code), fp)) <= 0) return 1;
    setup_interpret(&inter);
    interpret(&inter);

    fclose(fp);
    return 0;
}
