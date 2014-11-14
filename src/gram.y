%include
{
    #include <stdio.h>
    #include <string.h>
    #include <assert.h>
    #include <stdlib.h>
    #include "tree.h"
    #include "ast.h"
    #include "parser_state.h"
}

%token_type { const char * }
%token_destructor { free((char *)$$); }

%left ADD SUB.
%left MULT DIV.

%type atomic { struct node * }
%type primary_expression { struct node * }
%type negation { struct node * }
%type multiplication { struct node * }
%type multiplicative_expression { struct node * }
%type addition { struct node * }
%type additive_expression { struct node * }
%type expression { struct node * }
%type components { struct node * }
%type vector { struct node * }
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
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_TRANSLATION_UNIT;
    payload->alternative = ALT_DECLARATION_SEQUENCE;
    NODE = tree_create_node(payload, 1, DS);
    parser_state->root = NODE;
    parser_state->state = OK;
}
translation_unit ::= error.
{
    printf("%s", "Error handler\n");
}

declaration_sequence(NODE) ::= declaration(D) declaration_sequence(DS).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_DECLARATION_SEQUENCE;
    payload->alternative = ALT_DECLARATION_SEQUENCE;
    NODE = tree_create_node(payload, 2, D, DS);
}

declaration_sequence(NODE) ::= declaration(D).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_DECLARATION_SEQUENCE;
    payload->alternative = ALT_DECLARATION;
    NODE = tree_create_node(payload, 1, D);
}


declaration(NODE) ::= IDENTIFIER(I) EQ expression(AE) SEMIC.
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_DECLARATION;
    payload->alternative = ALT_EXPRESSION;
    payload->declaration.identifier = malloc(strlen(I) + 1);
    strcpy((char *)(payload->declaration.identifier), I);
    NODE = tree_create_node(payload, 1, AE);
    free((char *)I);
}


vector(NODE) ::= LBRACKET components(C) RBRACKET.
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_VECTOR;
    payload->alternative = ALT_COMPONENTS;
    NODE = malloc(sizeof(struct node) + sizeof(struct node *) * C->childc);
    NODE->payload = payload;
    NODE->childc = C->childc;
    memcpy(NODE->childv, C->childv, sizeof(struct node *) * C->childc);
    payload_free(C->payload);
    free(C);
}

components(NODE) ::= expression(AE) COMMA components(C).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_COMPONENTS;
    payload->alternative = ALT_EXPRESSION;
    NODE = malloc(sizeof(struct node) + sizeof(struct node *) * (C->childc + 1));
    NODE->payload = payload;
    NODE->childc = C->childc + 1;
    memcpy(NODE->childv, C->childv, sizeof(struct node *) * C->childc);
    NODE->childv[NODE->childc - 1] = AE;
    payload_free(C->payload);
    free(C);
}
components(NODE) ::= expression(AE).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_COMPONENTS;
    payload->alternative = ALT_EXPRESSION;
    NODE = tree_create_node(payload, 1, AE);
}

/** expressions */
expression(NODE) ::= additive_expression(ADE).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_EXPRESSION;
    payload->alternative = ALT_ADDITIVE_EXPRESSION;
    NODE = tree_create_node(payload, 1, ADE);
}

additive_expression(NODE) ::= addition(A).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_ADDITIVE_EXPRESSION;
    payload->alternative = ALT_ADDITION;
    NODE = tree_create_node(payload, 1, A);
}

additive_expression(NODE) ::= multiplicative_expression(ME).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_ADDITIVE_EXPRESSION;
    payload->alternative = ALT_MULTIPLICATIVE_EXPRESSION;
    NODE = tree_create_node(payload, 1, ME);
}

addition(NODE) ::= additive_expression(AE) ADD multiplicative_expression(ME).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_ADDITION;
    payload->alternative = ALT_ADD;
    NODE = tree_create_node(payload, 2, AE, ME);
}
addition(NODE) ::= additive_expression(AE) SUB multiplicative_expression(ME).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_ADDITION;
    payload->alternative = ALT_SUB;
    NODE = tree_create_node(payload, 2, AE, ME);
}

multiplicative_expression(NODE) ::= multiplication(M).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_MULTIPLICATIVE_EXPRESSION;
    payload->alternative = ALT_MULTIPLICATION;
    NODE = tree_create_node(payload, 1, M);
}
multiplicative_expression(NODE) ::= negation(N).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_MULTIPLICATIVE_EXPRESSION;
    payload->alternative = ALT_NEGATION;
    NODE = tree_create_node(payload, 1, N);
}

multiplication(NODE) ::= multiplicative_expression(ME) MULT negation(N).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_MULTIPLICATION;
    payload->alternative = ALT_MULT;
    NODE = tree_create_node(payload, 2, ME, N);
}
multiplication(NODE) ::= multiplicative_expression(ME) DIV negation(N).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_MULTIPLICATION;
    payload->alternative = ALT_DIV;
    NODE = tree_create_node(payload, 2, ME, N);
}

negation(NODE) ::= SUB negation(N).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_NEGATION;
    payload->alternative = ALT_NEGATION;
    NODE = tree_create_node(payload, 1, N);
}
negation(NODE) ::= primary_expression(PE).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_NEGATION;
    payload->alternative = ALT_PRIMARY_EXPRESSION;
    NODE = tree_create_node(payload, 1, PE);
}

primary_expression(NODE) ::= LPAREN expression(E) RPAREN.
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_PRIMARY_EXPRESSION;
    payload->alternative = ALT_EXPRESSION;
    NODE = tree_create_node(payload, 1, E);
}

primary_expression(NODE) ::= atomic(A).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_PRIMARY_EXPRESSION;
    payload->alternative = ALT_ATOMIC;
    NODE = tree_create_node(payload, 1, A);
}

atomic(NODE) ::= IDENTIFIER(I).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_ATOMIC;
    payload->alternative = ALT_IDENTIFIER;
    payload->atomic.identifier = malloc(strlen(I) + 1);
    strcpy((char *)(payload->atomic.identifier), I);
    NODE = tree_create_node(payload, 0);
    free((char *)I);
}

atomic(NODE) ::= NUMBER(N).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_ATOMIC;
    payload->alternative = ALT_NUMBER;
    payload->atomic.number = atof(N);
    NODE = tree_create_node(payload, 0);
    free((char *)N);
}

atomic(NODE) ::= vector(V).
{
    struct payload *payload = malloc(sizeof(struct payload));
    payload->type = N_ATOMIC;
    payload->alternative = ALT_VECTOR;
    NODE = tree_create_node(payload, 1, V);
}
