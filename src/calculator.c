#include <stdio.h>
#include <stdlib.h>
#include "gram.h"
#include "lexer.h"

void *ParseAlloc(void *(*allocProc) (size_t));
void Parse(void *, int, struct token *);
void ParseFree(void *, void (*freeProc) (void *));

int main()
{
/*
	char s[100];

	while (1) {
		fgets(s, 100, stdin);
        if (s[0] == '\n') {
            break;
        }
		struct lexer *lexer = alloc_lexer(s);
		if (lexer) {
			void *shellParser = ParseAlloc(malloc);
			struct token *t;
			while ((t = next_token(lexer))) {
				Parse(shellParser, t->value, t);
                t = NULL;
			}

			Parse(shellParser, 0, 0);     // Signal end of tokens
			ParseFree(shellParser, free);

			free_lexer(lexer);
		}
	}
	return 0;
*/
}
