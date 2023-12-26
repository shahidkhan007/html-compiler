#ifndef __scanner_h__
#define __scanner_h__

#include <stdbool.h>

typedef enum {
    CTX_INITIAL,
    CTX_IN_TAG,
    CTX_IN_ATTR,
} Ctx;

typedef struct {
    char* current;
    char* start;
    int line;
    Ctx ctx;
} Scanner;


typedef enum {
    TOKEN_TAG_START,
    TOKEN_TAG_END,
    TOKEN_SELF_CLOSING,
    TOKEN_TEXT,
    TOKEN_ATTR_NAME,
    TOKEN_ATTR_VALUE,
    TOKEN_ERROR,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char* start;
    int length;
} Token;

void init_scanner(Scanner* scanner, char* source);
void free_scanner(Scanner* scanner);
Token scan_token(Scanner* scanner);

#endif