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

translation_unit(TU) ::= declaration_sequence(DS).
{
  printf("%s", "translation_unit\n");
  TU = malloc(sizeof(struct translation_unit));
  TU->alternative = AST_DECLARATION_SEQUENCE;
  TU->declaration_sequence = DS;
}
translation_unit ::= error.
{
  printf("%s", "Error handler\n");
}

declaration_sequence(DSL) ::= declaration(D) declaration_sequence(DSR).
{
  printf("%s", "declaration_sequence\n");
  DSL = malloc(sizeof(struct declaration_sequence));
  DSL->alternative = AST_DECLARATION_SEQUENCE;
  DSL->declaration = D;
  DSL->declaration_sequence = DSR;
}
declaration_sequence(DS) ::= declaration(D).
{
  printf("%s", "declaration_sequence\n");
  DS = malloc(sizeof(struct declaration_sequence));
  DS->alternative = AST_DECLARATION;
  DS->declaration = D;
  DS->declaration_sequence = NULL;
}

declaration ::= vector_declaration SEMIC.
{
  printf("%s", "declaration\n");
}
declaration(D) ::= scalar_declaration(SD) SEMIC.
{
  printf("%s", "declaration\n");
  D = malloc(sizeof(struct declaration));
  D->alternative = AST_SCALAR_DECLARATION;
  D->scalar_declaration = SD;
}

vector_declaration           ::= IDENTIFIER EQ vector_expression. { printf("%s", "vector_declaration\n"); }

scalar_declaration(SD) ::= IDENTIFIER(I) EQ atomic_expression(AE).
{
  printf("%s", "scalar_declaration\n");
  SD = malloc(sizeof(struct scalar_declaration));
  SD->alternative = AST_ATOMIC_EXPRESSION;
  SD->identifier = I;
  SD->atomic_expression = AE;
}

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
atomic_expression(ATE) ::= additive_expression(ADE).
{
  printf("%s", "atomic_expression\n");
  ATE = malloc(sizeof(struct atomic_expression));
  ATE->alternative = AST_ADDITIVE_EXPRESSION;
  ATE->additive_expression = ADE;
}

additive_expression(AE) ::= addition(A).
{
  printf("%s", "additive_expression\n");
  AE = malloc(sizeof(struct additive_expression));
  AE->alternative = AST_ADDITION;
  AE->addition = A;
}
additive_expression(AE) ::= multiplicative_expression(ME).
{
  printf("%s", "additive_expression\n");
  AE = malloc(sizeof(struct additive_expression));
  AE->alternative = AST_MULTIPLICATIVE_EXPRESSION;
  AE->multiplicative_expression = ME;
}

addition(A) ::= multiplicative_expression(ME0) ADD multiplicative_expression(ME1).
{
  printf("%s", "addition\n");
  A = malloc(sizeof(struct addition));
  A->alternative = AST_ADD;
  A->expression0 = ME0;
  A->expression1 = ME1;
}
addition(A) ::= multiplicative_expression(ME0) SUB multiplicative_expression(ME1).
{
  printf("%s", "addition\n");
  A = malloc(sizeof(struct addition));
  A->alternative = AST_SUB;
  A->expression0 = ME0;
  A->expression1 = ME1;
}

multiplicative_expression(ME) ::= multiplication(M).
{
  printf("%s", "multiplicative_expression\n");
  ME = malloc(sizeof(struct multiplicative_expression));
  ME->alternative = AST_MULTIPLICATION;
  ME->multiplication = M;
  parser_state->state = OK;
}
multiplicative_expression(ME) ::= negation(N).
{
  printf("%s", "multiplicative_expression\n");
  ME = malloc(sizeof(struct multiplicative_expression));
  ME->alternative = AST_NEGATION;
  ME->negation = N;
  parser_state->state = OK;
}

multiplication(M) ::= negation(N0) MULT negation(N1).
{
  printf("%s", "multiplication\n");
  M = malloc(sizeof(struct multiplication));
  M->alternative = AST_MULTIPLICATION;
  M->negation0 = N0;
  M->negation1 = N1;
  parser_state->state = OK;
}
multiplication(M) ::= negation(N0) DIV negation(N1).
{
  printf("%s", "multiplication\n");
  M = malloc(sizeof(struct multiplication));
  M->alternative = AST_DIVISION;
  M->negation0 = N0;
  M->negation1 = N1;
  parser_state->state = OK;
}

negation(NL) ::= SUB negation(NR).
{
  printf("%s", "negation\n");
  NL = malloc(sizeof(struct negation));
  NL->alternative = AST_NEGATION;
  NL->negation = NR;
  parser_state->state = OK;
}
negation(N) ::= primary_expression(PE).
{
  printf("%s", "negation\n");
  N = malloc(sizeof(struct negation));
  N->alternative = AST_PRIMARY_EXPRESSION;
  N->primary_expression = PE;
  parser_state->state = OK;
}

primary_expression(PE) ::= LPAREN atomic_expression(AE) RPAREN.
{
  printf("%s", "(primary_expression)\n");
  PE = malloc(sizeof(struct primary_expression));
  PE->alternative = AST_ATOMIC_EXPRESSION;
  PE->atomic_expression = AE;
  parser_state->state = OK;
}
primary_expression(PE) ::= atomic(A).
{
  printf("%s", "primary_expression\n");
  PE = malloc(sizeof(struct primary_expression));
  PE->alternative = AST_ATOMIC;
  PE->atomic = A;
  parser_state->state = OK;
}

atomic(A) ::= IDENTIFIER(I).
{
  printf("%s", "atomic\n");
  A = malloc(sizeof(struct atomic));
  A->alternative = AST_IDENTIFIER;
  A->identifier = malloc(strlen(I) + 1);
  strcpy((char *)(A->identifier), I);
  parser_state->state = OK;
}
atomic(A) ::= NUMBER(N).
{
  printf("%s", "atomic\n");
  A = malloc(sizeof(struct atomic));
  A->alternative = AST_NUMBER;
  A->number = atof(N);
  parser_state->state = OK;
}
