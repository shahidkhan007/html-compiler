#include "scanner.h"
#include "common.h"
#include "debug.h"

void init_scanner(Scanner* scanner, char* source) {
    scanner->start = source;
    scanner->current = source;
    scanner->line = 1;
    scanner->ctx = CTX_INITIAL;
}

void free_scanner(Scanner* scanner) {
    scanner->start = NULL;
    scanner->current = NULL;
    scanner->line = 1;
}

static char peek(Scanner* scanner) {
    return *scanner->current;
}

static char advance(Scanner* scanner) {
    scanner->current++;
    return *(scanner->current - 1);
}

static Token make_token(Scanner* scanner, TokenType type) {
    Token t;
    t.type = type;
    t.start = scanner->start;
    t.length = (int)(scanner->current - scanner->start);

    return t;
}


static Token make_error_token(char* message) {
    Token t;
    t.type = TOKEN_ERROR;
    t.start = message;
    t.length = strlen(message);

    return t;
}

static bool is_at_end(Scanner* scanner) {
    return *(scanner->current) == '\0';
}

static Token _tag(Scanner* scanner) {
    Token t;

    scanner->start = scanner->current;
    for (;;) {
        char c = peek(scanner);

        if (c == ' ') {
            t = make_token(scanner, TOKEN_TAG_START);
            scanner->ctx = CTX_IN_TAG;
            break;
        } else if (c == '/') {
            t = make_token(scanner, TOKEN_SELF_CLOSING);

            // skip the '/' and '>'
            advance(scanner);
            advance(scanner);

            scanner->ctx = CTX_INITIAL;
            break;
        } else if (c == '>') {
            t = make_token(scanner, TOKEN_TAG_START);

            advance(scanner); // skip the '>'
            scanner->ctx = CTX_INITIAL;
            break;
        } else {
            advance(scanner);
        }
    }

    return t;
}

static Token closing_tag(Scanner* scanner) {
    for (;;) {
        char c = advance(scanner);

        if (c == '>') {
            scanner->ctx = CTX_INITIAL;
            return make_token(scanner, TOKEN_TAG_END);
        }
    }
}

static Token attribute(Scanner* scanner) {
    advance(scanner); // skip the whitespace
    scanner->start = scanner->current;
    Token t;

    for (;;) {
        char c = peek(scanner);

        if (c == ' ') {
            scanner->ctx = CTX_IN_TAG;
            t = make_token(scanner, TOKEN_ATTR_NAME);
            break;
        } else if (c == '>') {
            t = make_token(scanner, TOKEN_ATTR_NAME);
            advance(scanner);
            scanner->ctx = CTX_INITIAL;
            break;
        } else if (c == '/') {
            t = make_token(scanner, TOKEN_ATTR_NAME);
            advance(scanner);
            advance(scanner);
            scanner->ctx = CTX_INITIAL;
            break;
        } else if (c == '=') {
            t = make_token(scanner, TOKEN_ATTR_NAME);
            scanner->ctx = CTX_IN_ATTR;
            break;
        } else {
            advance(scanner);
        }
    }

    
    return t;
}

static Token attr_value(Scanner* scanner) {
    advance(scanner); // Skip the '='
    advance(scanner); // Skip the '"'
    scanner->start = scanner->current;

    for (;;) {
        char c = peek(scanner);

        if (c == '"') {
            break;
        } else {
            advance(scanner);
        }
    }

    Token t = make_token(scanner, TOKEN_ATTR_VALUE);
    advance(scanner); // skip the ending '"'

    char c = peek(scanner);

    if (c == ' ') {
        scanner->ctx = CTX_IN_TAG;
    } else if (c == '>') {
        advance(scanner);
        scanner->ctx = CTX_INITIAL;
    } else if (c == '/') {
        advance(scanner);
        advance(scanner);
        scanner->ctx = CTX_INITIAL;
    }
    
    return t;
}

static Token tag(Scanner* scanner) {
    char c = peek(scanner);

    if (c == '/') {
        return closing_tag(scanner);
    } else {
        return _tag(scanner);
    }
}

static Token text(Scanner* scanner) {
    for (;;) {
        char c = peek(scanner);

        if (c == '<') {
            break;
        } else {
            advance(scanner);
        }
    }

    return make_token(scanner, TOKEN_TEXT);
}

static void skip_whitespace(Scanner* scanner) {
    for (;;) {
        char c = peek(scanner);

        switch (c) {
            case '\n':
            case '\r':
            case '\t': {
                advance(scanner);
            }
            default:
                return;
        }
    }
}

static Token init(Scanner* scanner) {
    scanner->start = scanner->current;
    char c = advance(scanner);

    switch (c) {
        case '<': {
            return tag(scanner);
        }

        case '\0': {
            return make_token(scanner, TOKEN_EOF);
        }

        default: {
            return text(scanner);
        }
    }
}


Token scan_token(Scanner* scanner) {
    switch (scanner->ctx) {
        case CTX_INITIAL: {
            skip_whitespace(scanner);
            return init(scanner);
        }

        case CTX_IN_TAG: {
            // In tag, so scan for attributes
            return attribute(scanner);
        }

        case CTX_IN_ATTR: {
            // attribute name already read, read value next
            return attr_value(scanner);
        }

        default: {
            return make_error_token("Fatal Error: Tokenizer in an invalid state.");
        }
    }
}

