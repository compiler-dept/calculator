%include
{
    #include <stdio.h>
    #include <string.h>
    #include <assert.h>
    #include <stdlib.h>
    #include "lexer.h"
}

%syntax_error
{
    puts("Error parsing input.\n");
}

%token_type {struct token*}
%type list {
    struct {
        int size;
        char** elems;
    }
}

start ::= list(A) . 
{   
    for (int i = 0; i<A.size; i++){
        printf("Element %i: %s\n", i, A.elems[i]);
    }
    free(A.elems); 
}

list(A) ::= WORD(B) COMMA list(C) . 
{
    A.size = C.size + 1;
    A.elems = malloc((A.size) * sizeof(char*));
    A.elems[0] = B->text;
    for (int i = 1; i<A.size; i++){
        A.elems[i] = C.elems[i-1];
    }
    free(C.elems);
}

list(A) ::= WORD(B) . 
{ 
    A.size = 1;
    A.elems = malloc(sizeof(char*));
    A.elems[0] = B->text;
}
