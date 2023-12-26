#include "scanner.h"
#include "common.h"
#include "debug.h"

char* read_file(const char* name) {
    FILE* f;
    f = fopen(name, "r");

    if (f == NULL) {
        perror("Failed to open the file");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* file_mem = (char*)calloc(len, sizeof(char));

    size_t el_read = fread(file_mem, sizeof(char), len, f);

    fclose(f);

    return file_mem;
}

int main() {
    char* source = read_file("test.html");

    if (source == NULL) {
        return -1;
    }

    Scanner scanner;
    init_scanner(&scanner, source);

    Token token;

    for (;;) {
        token = scan_token(&scanner);
        print_token(token);

        if (token.type == TOKEN_EOF) {
            break;
        }
    }

    free_scanner(&scanner);
    free(source);
    return 0;
}