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
  TU = malloc(sizeof(struct translation_unit));
  TU->alternative = ALT_DECLARATION_SEQUENCE;
  TU->declaration_sequence = DS;
  parser_state->translation_unit = TU; 
}
translation_unit ::= error.
{
  printf("%s", "Error handler\n");
}

declaration_sequence(DSL) ::= declaration(D) declaration_sequence(DSR).
{
  DSL = malloc(sizeof(struct declaration_sequence));
  DSL->alternative = ALT_DECLARATION_SEQUENCE;
  DSL->declaration = D;
  DSL->declaration_sequence = DSR;
}
declaration_sequence(DS) ::= declaration(D).
{
  DS = malloc(sizeof(struct declaration_sequence));
  DS->alternative = ALT_DECLARATION;
  DS->declaration = D;
  DS->declaration_sequence = NULL;
}

declaration ::= vector_declaration SEMIC.
declaration(D) ::= scalar_declaration(SD) SEMIC.
{
  D = malloc(sizeof(struct declaration));
  D->alternative = ALT_SCALAR_DECLARATION;
  D->scalar_declaration = SD;
}

vector_declaration::= IDENTIFIER EQ vector_expression.

scalar_declaration(SD) ::= IDENTIFIER(I) EQ atomic_expression(AE).
{
  SD = malloc(sizeof(struct scalar_declaration));
  SD->alternative = ALT_ATOMIC_EXPRESSION;
  SD->identifier = I;
  SD->atomic_expression = AE;
}

/** vector */
vector_expression ::= vector_addition.

vector_addition ::= vector_scalar_multiplication VECADD vector_scalar_multiplication.
vector_addition ::= vector_scalar_multiplication.

vector_scalar_multiplication ::= atomic_expression SCMULT vector_negation.
vector_scalar_multiplication ::= vector_negation.

vector_negation ::= SUB vector_negation.
vector_negation ::= vector_primary_expression.

vector_primary_expression ::= vector.
vector_primary_expression ::= LPAREN vector_expression RPAREN.

vector ::= LBRACKET components RBRACKET.

components ::= atomic_expression COMMA components.
components ::= atomic_expression.

/** scalar */
atomic_expression(ATE) ::= additive_expression(ADE).
{
  ATE = malloc(sizeof(struct atomic_expression));
  ATE->alternative = ALT_ADDITIVE_EXPRESSION;
  ATE->additive_expression = ADE;
}

additive_expression(AE) ::= addition(A).
{
  AE = malloc(sizeof(struct additive_expression));
  AE->alternative = ALT_ADDITION;
  AE->addition = A;
}
additive_expression(AE) ::= multiplicative_expression(ME).
{
  AE = malloc(sizeof(struct additive_expression));
  AE->alternative = ALT_MULTIPLICATIVE_EXPRESSION;
  AE->multiplicative_expression = ME;
}

addition(A) ::= multiplicative_expression(ME0) ADD multiplicative_expression(ME1).
{
  A = malloc(sizeof(struct addition));
  A->alternative = ALT_ADD;
  A->expression0 = ME0;
  A->expression1 = ME1;
}
addition(A) ::= multiplicative_expression(ME0) SUB multiplicative_expression(ME1).
{
  A = malloc(sizeof(struct addition));
  A->alternative = ALT_SUB;
  A->expression0 = ME0;
  A->expression1 = ME1;
}

multiplicative_expression(ME) ::= multiplication(M).
{
  ME = malloc(sizeof(struct multiplicative_expression));
  ME->alternative = ALT_MULTIPLICATION;
  ME->multiplication = M;
  parser_state->state = OK;
}
multiplicative_expression(ME) ::= negation(N).
{
  ME = malloc(sizeof(struct multiplicative_expression));
  ME->alternative = ALT_NEGATION;
  ME->negation = N;
  parser_state->state = OK;
}

multiplication(M) ::= negation(N0) MULT negation(N1).
{
  M = malloc(sizeof(struct multiplication));
  M->alternative = ALT_MULT;
  M->negation0 = N0;
  M->negation1 = N1;
  parser_state->state = OK;
}
multiplication(M) ::= negation(N0) DIV negation(N1).
{
  M = malloc(sizeof(struct multiplication));
  M->alternative = ALT_DIV;
  M->negation0 = N0;
  M->negation1 = N1;
  parser_state->state = OK;
}

negation(NL) ::= SUB negation(NR).
{
  NL = malloc(sizeof(struct negation));
  NL->alternative = ALT_NEGATION;
  NL->negation = NR;
  parser_state->state = OK;
}
negation(N) ::= primary_expression(PE).
{
  N = malloc(sizeof(struct negation));
  N->alternative = ALT_PRIMARY_EXPRESSION;
  N->primary_expression = PE;
  parser_state->state = OK;
}

primary_expression(PE) ::= LPAREN atomic_expression(AE) RPAREN.
{
  PE = malloc(sizeof(struct primary_expression));
  PE->alternative = ALT_ATOMIC_EXPRESSION;
  PE->atomic_expression = AE;
  parser_state->state = OK;
}
primary_expression(PE) ::= atomic(A).
{
  PE = malloc(sizeof(struct primary_expression));
  PE->alternative = ALT_ATOMIC;
  PE->atomic = A;
  parser_state->state = OK;
}

atomic(A) ::= IDENTIFIER(I).
{
  A = malloc(sizeof(struct atomic));
  A->alternative = ALT_IDENTIFIER;
  A->identifier = malloc(strlen(I) + 1);
  strcpy((char *)(A->identifier), I);
  parser_state->state = OK;
}
atomic(A) ::= NUMBER(N).
{
  A = malloc(sizeof(struct atomic));
  A->alternative = ALT_NUMBER;
  A->number = atof(N);
  parser_state->state = OK;
}
