#include "lexer.h"

struct token *alloc_token(int value, int start, int end, const char *text)
{
	struct token *t = malloc(sizeof(struct token));
	t->value = value;
	t->text = malloc(strlen(text) + 1);
    t->text[strlen(text)] = '\0';
	strcpy(t->text, text);
	t->start = start;
	t->end = end;
	return t;
}

void free_token(struct token *t)
{
	free(t->text);
	free(t);
}

pcre *compile_regex(const char *regex)
{
	const char *error;
	int erroroffset;
	pcre *re;
	re = pcre_compile(regex, 0, &error, &erroroffset, NULL);
	if (re == NULL) {
		fprintf(stderr, "Error compiling REGEX_WHITESPACE: %s", error);
		return NULL;
	}

	return re;
}

struct lexer *alloc_lexer(const char *source)
{
	struct lexer *lexer = malloc(sizeof(struct lexer));
	lexer->source = source;
	lexer->pos = source;

	lexer->re_comma = compile_regex(REGEX_COMMA);
	lexer->re_text = compile_regex(REGEX_TEXT);

	if (!(lexer->re_comma && lexer->re_text)) {
		free_lexer(lexer);
		return NULL;
	}

	return lexer;
}

void free_lexer(struct lexer *lexer)
{
	if (lexer->re_comma) {
		pcre_free(lexer->re_comma);
	}
	if (lexer->re_text) {
		pcre_free(lexer->re_text);
	}
	free(lexer);
}

struct token *token_for_match(const char *source, int ovector[], int type)
{
	int len = ovector[3] - ovector[2];
	char match[len + 1];
	match[len] = '\0';
	strncpy(match, source + ovector[2], len);

	struct token *t = alloc_token(type, ovector[2], ovector[3], match);
	return t;
}

struct token *next_token(struct lexer *lexer)
{
	int rc;
	int ovector[30];

	struct token *token = NULL;

	rc = pcre_exec(lexer->re_comma, NULL, lexer->pos,
		       strlen(lexer->pos), 0, 0, ovector, 30);
	if (rc > 0) {
		token = token_for_match(lexer->pos, ovector, COMMA);
	} else {
		rc = pcre_exec(lexer->re_text, NULL, lexer->pos,
			       strlen(lexer->pos), 0, 0, ovector,
			       30);
		if (rc > 0) {
			token = token_for_match(lexer->pos, ovector, TEXT);
		}
	}

    if (token){
        lexer->pos += ovector[1];
    }

	return token;
}
