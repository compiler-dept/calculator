#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "gram.h"
#include "parser_state.h"

void *ParseAlloc(void *(*allocProc)(size_t));
void Parse(void *, int, const char *, struct parser_state *);
void ParseFree(void *, void (*freeProc)(void *));

int main()
{
	char s[1024];
	int r = 0;
	int token;
	struct parser_state parser_state;
	yyscan_t scanner;
	yylex_init(&scanner);

	while(1) {
		r = scanf("%s", s);
		YY_BUFFER_STATE bufferState = yy_scan_string(s, scanner);

		int token = yylex(scanner);
		void *shellParser = ParseAlloc(malloc);
		while(token > 0) {
			Parse(shellParser, token, yyget_text(scanner), &parser_state);
    	token = yylex(scanner);
  	}

		Parse(shellParser, 0, NULL, &parser_state);
		ParseFree(shellParser, free);
	}

	return 0;
}
