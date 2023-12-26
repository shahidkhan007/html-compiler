#include "debug.h"

#include <stdio.h>
#include <string.h>

static void print_string(char* string, int length) {
    for (int i = 0; i < length; i++) {
        printf("%c", string[i]);
    }
}

void print_token(Token token) {
    switch (token.type) {
        case TOKEN_TAG_START: {
            printf("[%-10s] ", "+TAG");
            print_string(token.start, token.length);
            printf("\n");
            break;
        }

        case TOKEN_TAG_END: {
            printf("[%-10s] ", "-TAG");
            print_string(token.start, token.length);
            printf("\n");
            break;
        }

        case TOKEN_SELF_CLOSING: {
            printf("[%-10s] ", "TAG-SC");
            print_string(token.start, token.length);
            printf("\n");
            break;
        }

        case TOKEN_TEXT: {
            printf("[%-10s] ", "TEXT");
            print_string(token.start, token.length);
            printf("\n");
            break;
        }

        case TOKEN_ATTR_NAME: {
            printf("[%-10s] ", "ATTR-NAME");
            print_string(token.start, token.length);
            printf("\n");
            break;
        }

        case TOKEN_ATTR_VALUE: {
            printf("[%-10s] ", "ATTR-VALUE");
            print_string(token.start, token.length);
            printf("\n");
            break;
        }

        case TOKEN_EOF: {
            printf("[%-10s] EOF\n", "EOF");
            break;
        }

        case TOKEN_ERROR: {
            printf("Error: %s\n", token.start);
            break;
        }

        default: {
            printf("Unknown Token (%s)\n", token.start);
            break;
        }
    }
}
