#ifndef AST_H
#define AST_H

#include "stack.h"

enum type {
	N_ATOMIC = 1,
	N_PRIMARY_EXPRESSION,
	N_NEGATION,
	N_MULTIPLICATION,
	N_MULTIPLICATIVE_EXPRESSION,
	N_ADDITION,
	N_ADDITIVE_EXPRESSION,
	N_ATOMIC_EXPRESSION,
	N_COMPONENTS,
	N_VECTOR,
	N_VECTOR_PROMARY_EXPRESSION,
	N_VECTOR_NEGATION,
	N_VECTOR_SCALAR_MULTIPLICATION,
	N_VECTOR_ADDITION,
	N_VECTOR_EXPRESSION,
	N_SCALAR_DECLARATION,
	N_VECTOR_DECLARATION,
	N_DECLARATION,
	N_DECLARATION_SEQUENCE,
	N_TRANSLATION_UNIT
};

enum alternative {
	ALT_ATOMIC = 1,
	ALT_PRIMARY_EXPRESSION,
	ALT_NEGATION,
	ALT_MULTIPLICATION,
	ALT_MULTIPLICATIVE_EXPRESSION,
	ALT_ADDITION,
	ALT_ADDITIVE_EXPRESSION,
	ALT_ATOMIC_EXPRESSION,
	ALT_COMPONENTS,
	ALT_VECTOR,
	ALT_VECTOR_PROMARY_EXPRESSION,
	ALT_VECTOR_NEGATION,
	ALT_VECTOR_SCALAR_MULTIPLICATION,
	ALT_VECTOR_ADDITION,
	ALT_VECTOR_EXPRESSION,
	ALT_SCALAR_DECLARATION,
	ALT_VECTOR_DECLARATION,
	ALT_DECLARATION,
	ALT_DECLARATION_SEQUENCE,
	ALT_TRANSLATION_UNIT,

	ALT_IDENTIFIER,
	ALT_NUMBER,

	ALT_ADD,
	ALT_SUB,
	ALT_MULT,
	ALT_DIV
};

struct atomic {
	union {
		double number;
		const char *identifier;
	};
};

struct primary_expression {
};

struct negation {
};

struct multiplication {
};

struct multiplicative_expression {
};

struct addition {
};

struct additive_expression {
};

struct atomic_expression {
};

struct scalar_declaration {
	const char *identifier;
};

struct declaration {
};



struct payload {
    union {
        struct atomic atomic;
        struct primary_expression primary_expression;
        struct negation negation;
        struct multiplication multiplication;
        struct multiplicative_expression multiplicative_expression;
        struct addition addition;
        struct additive_expression additive_expression;
        struct atomic_expression atomic_expression;
        struct scalar_declaration scalar_declaration;
        struct declaration declaration;
        struct declaration_sequence declaration_sequence;
        struct atomic_expression atomic_expression;
    }
}

struct node {
    enum type type;
    enum alternative alternative;
    struct payload payload;
    int childc;
    struct node **childv;
}

/**
 * AST iterator
 */

struct ast_iterator {
	struct stack *stack;
	void *current;
};

struct ast_iterator *ast_iterator_init(struct translation_unit *);
void *ast_iterator_next(struct ast_iterator *);

#endif
