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
%token_destructor { free((char *)$$); }

%left ADD SUB.
%left MULT DIV.

%type atomic { struct node * }
%type vector_atomic { struct node * }
%type primary_expression { struct node * }
%type negation { struct node * }
%type multiplication { struct node * }
%type multiplicative_expression { struct node * }
%type addition { struct node * }
%type additive_expression { struct node * }
%type atomic_expression { struct node * }
%type vector_expression { struct node * }
%type vector_addition { struct node * }
%type vector_scalar_multiplication { struct node * }
%type vector_negation { struct node * }
%type vector_primary_expression { struct node * }
%type components { struct node * }
%type vector { struct node * }
%type vector_declaration { struct node * }
%type scalar_declaration { struct node * }
%type declaration { struct node * }
%type declaration_sequence { struct node * }
%type translation_unit { struct node * }

%extra_argument { struct parser_state *parser_state }

%syntax_error
{
    puts("Error parsing input.");
    parser_state->state = ERROR;
}

translation_unit(NODE) ::= declaration_sequence(DS).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_TRANSLATION_UNIT;
  NODE->alternative = ALT_DECLARATION_SEQUENCE;
  NODE->childc = 1;
  NODE->childv = malloc(sizeof(struct node *));
  NODE->childv[0] = DS;
  parser_state->root = NODE;
  parser_state->state = OK;
}
translation_unit ::= error.
{
  printf("%s", "Error handler\n");
}

declaration_sequence(NODE) ::= declaration(D) declaration_sequence(DS).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_DECLARATION_SEQUENCE;
  NODE->alternative = ALT_DECLARATION_SEQUENCE;
  NODE->childc = 2;
  NODE->childv = malloc(2 * sizeof(struct node *));
  NODE->childv[0] = D;
  NODE->childv[1] = DS;
}

declaration_sequence(NODE) ::= declaration(D).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_DECLARATION_SEQUENCE;
  NODE->alternative = ALT_DECLARATION;
  NODE->childc = 1;
  NODE->childv = malloc(sizeof(struct node *));
  NODE->childv[0] = D;
}

declaration(NODE) ::= VEC vector_declaration(VD) SEMIC.
{
    NODE = malloc(sizeof(struct node));
    NODE->childv = NULL;
    NODE->type = N_DECLARATION;
    NODE->alternative = ALT_VECTOR_DECLARATION;
    NODE->childc = 1;
    NODE->childv = malloc(sizeof(struct node *));
    NODE->childv[0] = VD;
}
declaration(NODE) ::= scalar_declaration(SD) SEMIC.
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_DECLARATION;
  NODE->alternative = ALT_SCALAR_DECLARATION;
  NODE->childc = 1;
  NODE->childv = malloc(sizeof(struct node *));
  NODE->childv[0] = SD;
}

vector_declaration(NODE) ::= IDENTIFIER(I) EQ vector_expression(VE).
{
    NODE = malloc(sizeof(struct node));
    NODE->childv = NULL;
    NODE->type = N_VECTOR_DECLARATION;
    NODE->alternative = ALT_VECTOR_EXPRESSION;
    NODE->payload.vector_declaration.identifier = malloc(strlen(I) + 1);
    strcpy((char *)(NODE->payload.vector_declaration.identifier), I);
    NODE->childc = 1;
    NODE->childv = malloc(sizeof(struct node *));
    NODE->childv[0] = VE;
    free((char *)I);
}

scalar_declaration(NODE) ::= IDENTIFIER(I) EQ atomic_expression(AE).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_SCALAR_DECLARATION;
  NODE->alternative = ALT_ATOMIC_EXPRESSION;
  NODE->payload.scalar_declaration.identifier = malloc(strlen(I) + 1);
  strcpy((char *)(NODE->payload.scalar_declaration.identifier), I);
  NODE->childc = 1;
  NODE->childv = malloc(sizeof(struct node *));
  NODE->childv[0] = AE;
  free((char *)I);
}

/** vector */
vector_expression(NODE) ::= vector_addition(VE).
{
    NODE = malloc(sizeof(struct node));
    NODE->childv = NULL;
    NODE->type = N_VECTOR_EXPRESSION;
    NODE->alternative = ALT_VECTOR_ADDITION;
    NODE->childc = 1;
    NODE->childv = malloc(sizeof(struct node *));
    NODE->childv[0] = VE;
}

vector_addition(NODE) ::= vector_addition(VA) VECADD vector_scalar_multiplication(VSM).
{
    NODE = malloc(sizeof(struct node));
    NODE->childv = NULL;
    NODE->type = N_VECTOR_ADDITION;
    NODE->alternative = ALT_VECADD;
    NODE->childc = 2;
    NODE->childv = malloc(NODE->childc * sizeof(struct node *));
    NODE->childv[0] = VA;
    NODE->childv[1] = VSM;
}
vector_addition(NODE) ::= vector_scalar_multiplication(VSM).
{
    NODE = malloc(sizeof(struct node));
    NODE->childv = NULL;
    NODE->type = N_VECTOR_ADDITION;
    NODE->alternative = ALT_VECTOR_SCALAR_MULTIPLICATION;
    NODE->childc = 1;
    NODE->childv = malloc(sizeof(struct node *));
    NODE->childv[0] = VSM;
}

vector_scalar_multiplication(NODE) ::= atomic_expression(AE) SCMULT vector_negation(VN).
{
    NODE = malloc(sizeof(struct node));
    NODE->childv = NULL;
    NODE->type = N_VECTOR_SCALAR_MULTIPLICATION;
    NODE->alternative = ALT_SCMULT;
    NODE->childc = 2;
    NODE->childv = malloc(NODE->childc * sizeof(struct node *));
    NODE->childv[0] = AE;
    NODE->childv[1] = VN;
}
vector_scalar_multiplication(NODE) ::= vector_negation(VN).
{
    NODE = malloc(sizeof(struct node));
    NODE->childv = NULL;
    NODE->type = N_VECTOR_SCALAR_MULTIPLICATION;
    NODE->alternative = ALT_VECTOR_NEGATION;
    NODE->childc = 1;
    NODE->childv = malloc(sizeof(struct node *));
    NODE->childv[0] = VN;
}

vector_negation(NODE) ::= SUB vector_negation(VN).
{
    NODE = malloc(sizeof(struct node));
    NODE->childv = NULL;
    NODE->type = N_VECTOR_NEGATION;
    NODE->alternative = ALT_VECTOR_NEGATION;
    NODE->childc = 1;
    NODE->childv = malloc(sizeof(struct node *));
    NODE->childv[0] = VN;
}
vector_negation(NODE) ::= vector_primary_expression(VPE).
{
    NODE = malloc(sizeof(struct node));
    NODE->childv = NULL;
    NODE->type = N_VECTOR_NEGATION;
    NODE->alternative = ALT_VECTOR_PRIMARY_EXPRESSION;
    NODE->childc = 1;
    NODE->childv = malloc(sizeof(struct node *));
    NODE->childv[0] = VPE;
}

vector_primary_expression(NODE) ::= vector_atomic(VA).
{
    NODE = malloc(sizeof(struct node));
    NODE->childv = NULL;
    NODE->type = N_VECTOR_PRIMARY_EXPRESSION;
    NODE->alternative = ALT_VECTOR_ATOMIC;
    NODE->childc = 1;
    NODE->childv = malloc(sizeof(struct node *));
    NODE->childv[0] = VA;
}
vector_primary_expression(NODE) ::= LPAREN vector_expression(VE) RPAREN.
{
    NODE = malloc(sizeof(struct node));
    NODE->childv = NULL;
    NODE->type = N_VECTOR_PRIMARY_EXPRESSION;
    NODE->alternative = ALT_VECTOR_EXPRESSION;
    NODE->childc = 1;
    NODE->childv = malloc(sizeof(struct node *));
    NODE->childv[0] = VE;
}

vector_atomic(NODE) ::= IDENTIFIER(I).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_VECTOR_ATOMIC;
  NODE->alternative = ALT_IDENTIFIER;
  NODE->payload.vector_atomic.identifier = malloc(strlen(I) + 1);
  strcpy((char *)(NODE->payload.vector_atomic.identifier), I);
  NODE->childc = 0;
  free((char *)I);
}
vector_atomic(NODE) ::= vector(V).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_VECTOR_ATOMIC;
  NODE->alternative = ALT_VECTOR;
  NODE->childc = 1;
  NODE->childv = malloc(sizeof(struct node *));
  NODE->childv[0] = V;
}

vector(NODE) ::= LBRACKET components(C) RBRACKET.
{
    NODE = malloc(sizeof(struct node));
    NODE->childv = NULL;
    NODE->type = N_VECTOR;
    NODE->alternative = ALT_COMPONENTS;
    NODE->childc = C->childc;
    NODE->childv = C->childv;
    free(C);
}

components(NODE) ::= atomic_expression(AE) COMMA components(C).
{
    NODE = malloc(sizeof(struct node));
    NODE->childv = NULL;
    NODE->type = N_COMPONENTS;
    NODE->alternative = ALT_ATOMIC_EXPRESSION;
    NODE->childc = C->childc + 1;
    NODE->childv = realloc(C->childv, NODE->childc * sizeof(struct node *));
    NODE->childv[NODE->childc - 1] = AE;
    free(C);
}
components(NODE) ::= atomic_expression(AE).
{
    NODE = malloc(sizeof(struct node));
    NODE->childv = NULL;
    NODE->type = N_COMPONENTS;
    NODE->alternative = ALT_ATOMIC_EXPRESSION;
    NODE->childc = 1;
    NODE->childv = malloc(sizeof(struct node *));
    NODE->childv[0] = AE;
}

/** scalar */
atomic_expression(NODE) ::= additive_expression(ADE).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_ATOMIC_EXPRESSION;
  NODE->alternative = ALT_ADDITIVE_EXPRESSION;
  NODE->childc = 1;
  NODE->childv = malloc(sizeof(struct node *));
  NODE->childv[0] = ADE;
}

additive_expression(NODE) ::= addition(A).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_ADDITIVE_EXPRESSION;
  NODE->alternative = ALT_ADDITION;
  NODE->childc = 1;
  NODE->childv = malloc(sizeof(struct node *));
  NODE->childv[0] = A;
}

additive_expression(NODE) ::= multiplicative_expression(ME).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_ADDITIVE_EXPRESSION;
  NODE->alternative = ALT_MULTIPLICATIVE_EXPRESSION;
  NODE->childc = 1;
  NODE->childv = malloc(sizeof(struct node *));
  NODE->childv[0] = ME;
}

addition(NODE) ::= additive_expression(AE) ADD multiplicative_expression(ME).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_ADDITION;
  NODE->alternative = ALT_ADD;
  NODE->childc = 2;
  NODE->childv = malloc(2 * sizeof(struct node *));
  NODE->childv[0] = AE;
  NODE->childv[1] = ME;
}
addition(NODE) ::= additive_expression(AE) SUB multiplicative_expression(ME).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_ADDITION;
  NODE->alternative = ALT_SUB;
  NODE->childc = 2;
  NODE->childv = malloc(2 * sizeof(struct node *));
  NODE->childv[0] = AE;
  NODE->childv[1] = ME;
}

multiplicative_expression(NODE) ::= multiplication(M).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_MULTIPLICATIVE_EXPRESSION;
  NODE->alternative = ALT_MULTIPLICATION;
  NODE->childc = 1;
  NODE->childv = malloc(sizeof(struct node *));
  NODE->childv[0] = M;
}
multiplicative_expression(NODE) ::= negation(N).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_MULTIPLICATIVE_EXPRESSION;
  NODE->alternative = ALT_NEGATION;
  NODE->childc = 1;
  NODE->childv = malloc(sizeof(struct node *));
  NODE->childv[0] = N;
}

multiplication(NODE) ::= multiplicative_expression(ME) MULT negation(N).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_MULTIPLICATION;
  NODE->alternative = ALT_MULT;
  NODE->childc = 2;
  NODE->childv = malloc(2 * sizeof(struct node *));
  NODE->childv[0] = ME;
  NODE->childv[1] = N;
}
multiplication(NODE) ::= multiplicative_expression(ME) DIV negation(N).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_MULTIPLICATION;
  NODE->alternative = ALT_DIV;
  NODE->childc = 2;
  NODE->childv = malloc(2 * sizeof(struct node *));
  NODE->childv[0] = ME;
  NODE->childv[1] = N;
}

negation(NODE) ::= SUB negation(NR).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_NEGATION;
  NODE->alternative = ALT_NEGATION;
  NODE->childc = 1;
  NODE->childv = malloc(sizeof(struct node *));
  NODE->childv[0] = NR;
}
negation(NODE) ::= primary_expression(PE).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_NEGATION;
  NODE->alternative = ALT_PRIMARY_EXPRESSION;
  NODE->childc = 1;
  NODE->childv = malloc(sizeof(struct node *));
  NODE->childv[0] = PE;
}

primary_expression(NODE) ::= LPAREN atomic_expression(AE) RPAREN.
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_PRIMARY_EXPRESSION;
  NODE->alternative = ALT_ATOMIC_EXPRESSION;
  NODE->childc = 1;
  NODE->childv = malloc(sizeof(struct node *));
  NODE->childv[0] = AE;
}

primary_expression(NODE) ::= atomic(A).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_PRIMARY_EXPRESSION;
  NODE->alternative = ALT_ATOMIC;
  NODE->childc = 1;
  NODE->childv = malloc(sizeof(struct node *));
  NODE->childv[0] = A;
}

atomic(NODE) ::= IDENTIFIER(I).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_ATOMIC;
  NODE->alternative = ALT_IDENTIFIER;
  NODE->payload.atomic.identifier = malloc(strlen(I) + 1);
  strcpy((char *)(NODE->payload.atomic.identifier), I);
  NODE->childc = 0;
  free((char *)I);
}

atomic(NODE) ::= NUMBER(N).
{
  NODE = malloc(sizeof(struct node));
  NODE->childv = NULL;
  NODE->type = N_ATOMIC;
  NODE->alternative = ALT_NUMBER;
  NODE->payload.atomic.number = atof(N);
  NODE->childc = 0;
  free((char *)N);
}
