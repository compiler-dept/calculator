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

negation                     ::= SUB negation. { printf("%s", "negation\n"); }
negation                     ::= primary_expression. { printf("%s", "negation\n"); }

primary_expression           ::= LPAREN atomic_expression RPAREN. { printf("%s", "(primary_expression)\n"); }
primary_expression           ::= atomic. { printf("%s", "primary_expression\n"); }

atomic(ATOMIC)               ::= IDENTIFIER(VALUE).
{
  printf("%s", "atomic\n");
  ATOMIC = malloc(sizeof(struct atomic));
  ATOMIC->type = AST_IDENTIFIER;
  ATOMIC->identifier = malloc(strlen(VALUE) + 1);
  strcpy((char *)(ATOMIC->identifier), VALUE);
  parser_state->atomic = ATOMIC;
  parser_state->state = OK;
}
atomic(ATOMIC)               ::= NUMBER(VALUE).
{
  printf("%s", "atomic\n");
  ATOMIC = malloc(sizeof(struct atomic));
  ATOMIC->type = AST_NUMBER;
  ATOMIC->number = atof(VALUE);
  parser_state->atomic = ATOMIC;
  parser_state->state = OK;
}
