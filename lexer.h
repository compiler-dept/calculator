#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gram.h"
#include "pcre.h"

#define REGEX_COMMA "^\\s*(,)"
#define REGEX_TEXT "^\\s*([^,\\n]+)"

struct lexer {
	const char *source;
	const char *pos;
	pcre *re_comma;
	pcre *re_text;
};

struct token {
	int value;
	int start;
	int end;
	char *text;
};

struct token *alloc_token(int value, int start, int end, const char *text);
void free_token(struct token *t);

struct lexer *alloc_lexer(const char *source);
void free_lexer(struct lexer *lexer);

struct token *next_token(struct lexer *lexer);

#endif
