#include <stdio.h>
#include <stdlib.h>
#include "list.h"
 
void* ParseAlloc(void* (*allocProc)(size_t));
void Parse(void*, int, const char*);
void ParseFree(void*, void(*freeProc)(void*));
 
int main()
{
    void* shellParser = ParseAlloc(malloc);
    Parse(shellParser, WORD, "hallo");
    Parse(shellParser, COMMA, 0);
    Parse(shellParser, WORD, "welt");
    Parse(shellParser, COMMA, 0);
    Parse(shellParser, WORD, "foo");
    Parse(shellParser, 0, 0); // Signal end of tokens
    ParseFree(shellParser, free);
    return 0;
}
