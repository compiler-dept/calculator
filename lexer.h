#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "pcre.h"

struct token {
    int value;
    int start;
    int end;
    char* text;
};


struct token *alloc_token(int value, int start, int end, const char *text);
void free_token(struct token *t);
struct token *next_token(char *s, int offset);

#endif
