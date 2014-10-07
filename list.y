%include
{
    #include <stdio.h>
    #include <string.h>
    #include <assert.h>
    #include <stdlib.h>
}

%syntax_error
{
    puts("Error parsing input.\n");
}

%token_type {char*}

start ::= list(A) . { printf("[%s]\n", A); free(A); }

list(A) ::= WORD(B) COMMA list(C) . 
{
    A = malloc(strlen(B)+strlen(C)+3); 
    strcat(A, B); 
    strcat(A, ", "); 
    strcat(A, C);
}
list(A) ::= WORD(B) . { A = B; }
