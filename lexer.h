#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <string.h>
#include "list.h"

struct token {
    int value;
    char* text;
};

struct lexer {
    int position;
};

struct token *alloc_token(int value, char* text);
void free_token(struct token *t);

#endif
