%include
{
    #include <stdio.h>
    #include <string.h>
    #include <assert.h>
    #include <stdlib.h>
    #include "ast.h"
    #include "parser_state.h"
}

%token_type { const char * }

%type atomic { struct atomic * }
%type primary_expression { struct primary_expression * }
%type negation { struct negation * }
%type multiplication { struct multiplication * }
%type multiplicative_expression { struct multiplicative_expression * }
%type addition { struct addition * }
%type additive_expression { struct additive_expression * }
%type atomic_expression { struct atomic_expression * }
%type scalar_declaration { struct scalar_declaration * }
%type declaration { struct declaration * }
%type declaration_sequence { struct declaration_sequence * }
%type translation_unit { struct translation_unit * }

%extra_argument { struct parser_state *parser_state }

%syntax_error
{
    puts("Error parsing input.");
    parser_state->state = ERROR;
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

negation(NL) ::= SUB negation(NR).
{
  printf("%s", "negation\n");
  NL = malloc(sizeof(struct negation));
  NL->type = AST_NEGATION;
  NL->negation = NR;
  parser_state->negation = NL;
  parser_state->state = OK;
}
negation(N) ::= primary_expression(PE).
{
  printf("%s", "negation\n");
  N = malloc(sizeof(struct negation));
  N->type = AST_PRIMARY_EXPRESSION;
  N->primary_expression = PE;
  parser_state->negation = N;
  parser_state->state = OK;
}

primary_expression(PE) ::= LPAREN atomic_expression(AE) RPAREN.
{
  printf("%s", "(primary_expression)\n");
  PE = malloc(sizeof(struct primary_expression));
  PE->type = AST_ATOMIC_EXPRESSION;
  PE->atomic_expression = AE;
  parser_state->state = OK;
}
primary_expression(PE) ::= atomic(A).
{
  printf("%s", "primary_expression\n");
  PE = malloc(sizeof(struct primary_expression));
  PE->type = AST_ATOMIC;
  PE->atomic = A;
  parser_state->state = OK;
}

atomic(A) ::= IDENTIFIER(I).
{
  printf("%s", "atomic\n");
  A = malloc(sizeof(struct atomic));
  A->type = AST_IDENTIFIER;
  A->identifier = malloc(strlen(I) + 1);
  strcpy((char *)(A->identifier), I);
  parser_state->state = OK;
}
atomic(A) ::= NUMBER(N).
{
  printf("%s", "atomic\n");
  A = malloc(sizeof(struct atomic));
  A->type = AST_NUMBER;
  A->number = atof(N);
  parser_state->state = OK;
}
