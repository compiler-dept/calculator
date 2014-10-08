#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "lexer.h"
 
void* ParseAlloc(void* (*allocProc)(size_t));
void Parse(void*, int, struct token*);
void ParseFree(void*, void(*freeProc)(void*));
 
int main()
{
    void* shellParser = ParseAlloc(malloc);
    

    char *s = "hallo, welt, foo";

    struct token *t1 = next_token(s, 0);
    struct token *t2 = next_token(s, t1->end);
    struct token *t3 = next_token(s, t2->end);
    struct token *comma = alloc_token(COMMA, 0, 1, "");

    Parse(shellParser, t1->value, t1);
    Parse(shellParser, comma->value, comma);
    Parse(shellParser, t2->value, t2);
    Parse(shellParser, comma->value, comma);
    Parse(shellParser, t3->value, t3);
    Parse(shellParser, 0, 0); // Signal end of tokens
    ParseFree(shellParser, free);

    free_token(t1);
    free_token(t2);
    free_token(t3);
    free_token(comma);
    return 0;
}
