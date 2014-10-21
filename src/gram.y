%include
{
    #include <stdio.h>
    #include <string.h>
    #include <assert.h>
    #include <stdlib.h>
}

%token_type { const char * }
%extra_argument { char *error }

%syntax_error
{
    puts("Error parsing input.");
    *error = 1;
}

translation_unit             ::= declaration_sequence. { printf("%s", "translation_unit\n"); }
translation_unit             ::= error. { printf("%s", "Error handler\n"); }

declaration_sequence         ::= declaration declaration_sequence. { printf("%s", "declaration_sequence\n"); }
declaration_sequence         ::= declaration. { printf("%s", "declaration_sequence\n"); }

declaration                  ::= vector_declaration SEMIC. { printf("%s", "declaration\n"); }
declaration                  ::= scalar_declaration SEMIC. { printf("%s", "declaration\n"); }

vector_declaration           ::= IDENTIFIER EQ vector_expression. { printf("%s", "vector_declaration\n"); }

scalar_declaration           ::= IDENTIFIER EQ atomic_expression. { printf("%s", "scalar_declaration\n"); }

/** vector */
vector_expression            ::= vector_addition. { printf("%s", "vector_expression\n"); }

vector_addition              ::= vector_scalar_multiplication VECADD vector_scalar_multiplication. { printf("%s", "vector_addition\n"); }
vector_addition              ::= vector_scalar_multiplication. { printf("%s", "vector_addition\n"); }

vector_scalar_multiplication ::= atomic_expression SCMULT vector_negation. { printf("%s", "vector_scalar_multiplication\n"); }
vector_scalar_multiplication ::= vector_negation. { printf("%s", "vector_scalar_multiplication\n"); }

vector_negation              ::= SUB vector_negation. { printf("%s", "vector_negation\n"); }
vector_negation              ::= vector_primary_expression. { printf("%s", "vector_negation\n"); }

vector_primary_expression    ::= vector. { printf("%s", "vector_primary_expression\n"); }
vector_primary_expression    ::= LPAREN vector_expression RPAREN. { printf("%s", "vector_primary_expression\n"); }

vector                       ::= LBRACKET components RBRACKET. { printf("%s", "vector\n"); }

components                   ::= atomic_expression COMMA components. { printf("%s", "components\n"); }
components                   ::= atomic_expression. { printf("%s", "components\n"); }

/** scalar */
atomic_expression            ::= additive_expression. { printf("%s", "atomic_expression\n"); }

additive_expression          ::= addition. { printf("%s", "additive_expression\n"); }
additive_expression          ::= multiplicative_expression. { printf("%s", "additive_expression\n"); }

addition                     ::= multiplicative_expression ADD multiplicative_expression. { printf("%s", "addition\n"); }
addition                     ::= multiplicative_expression SUB multiplicative_expression. { printf("%s", "addition\n"); }

multiplicative_expression    ::= multiplication. { printf("%s", "multiplicative_expression\n"); }
multiplicative_expression    ::= negation. { printf("%s", "multiplicative_expression\n"); }

multiplication               ::= negation MULT negation. { printf("%s", "multiplication\n"); }
multiplication               ::= negation DIV negation. { printf("%s", "multiplication\n"); }

negation                     ::= SUB negation. { printf("%s", "negation\n"); }
negation                     ::= primary_expression. { printf("%s", "negation\n"); }

primary_expression           ::= LPAREN atomic_expression RPAREN. { printf("%s", "(primary_expression)\n"); }
primary_expression           ::= atomic. { printf("%s", "primary_expression\n"); }

atomic                       ::= IDENTIFIER. { printf("%s", "atomic\n"); }
atomic                       ::= NUMBER. { printf("%s", "atomic\n"); }