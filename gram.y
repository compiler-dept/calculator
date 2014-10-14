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

%token_type {struct token *}
%token_destructor { free_token($$); }

translation_unit             ::= declaration_sequence.

declaration_sequence         ::= declaration declaration_sequence.
declaration_sequence         ::= declaration.

declaration                  ::= vector_declaration SEMIC.
declaration                  ::= scalar_declaration SEMIC.

/** vector */
vector_declaration           ::= IDENTIFIER EQ vector_expression.

scalar_declaration           ::= IDENTIFIER EQ atomic_expression.

vector_expression            ::= vector_addition.

vector_addition              ::= vector_scalar_multiplication VECADD vector_scalar_multiplication.
vector_addition              ::= vector_scalar_multiplication.

vector_scalar_multiplication ::= atomic_expression SCMULT vector_negation.
vector_scalar_multiplication ::= vector_negation.

vector_negation              ::= SUB vector_negation.
vector_negation              ::= vector_primary_expression.

vector_primary_expression    ::= vector.
vector_primary_expression    ::= LPAREN vector_expression RPAREN.

vector                       ::= LBRACKET components RBRACKET.

components                   ::= atomic_expression COMMA components.
components                   ::= atomic_expression.

/** scalar */
atomic_expression            ::= additive_expression.

additive_expression          ::= addition.
additive_expression          ::= multiplicative_expression.

addition                     ::= multiplication ADD multiplication.
addition                     ::= multiplication SUB multiplication.

multiplicative_expression    ::= multiplication.
multiplicative_expression    ::= negation.

multiplication               ::= negation MULT negation.
multiplication               ::= negation DIV negation.

negation                     ::= SUB negation.
negation                     ::= primary_expression.

primary_expression           ::= LPAREN atomic_expression RPAREN.
primary_expression           ::= atomic.

atomic                       ::= IDENTIFIER.
atomic                       ::= NUMBER.
