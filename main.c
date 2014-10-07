#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "lexer.h"
 
void* ParseAlloc(void* (*allocProc)(size_t));
void Parse(void*, int, const char*);
void ParseFree(void*, void(*freeProc)(void*));
 
int main()
{
    void* shellParser = ParseAlloc(malloc);
    

    struct token *t1 = alloc_token(WORD, "hallo");
    struct token *t2 = alloc_token(WORD, "welt");
    struct token *t3 = alloc_token(WORD, "foo");
    struct token *comma = alloc_token(COMMA, "");


    Parse(shellParser, t1->value, t1->text);
    Parse(shellParser, comma->value, comma->text);
    Parse(shellParser, t2->value, t2->text);
    Parse(shellParser, comma->value, comma->text);
    Parse(shellParser, t3->value, t3->text);
    Parse(shellParser, 0, 0); // Signal end of tokens
    ParseFree(shellParser, free);
    return 0;
}
