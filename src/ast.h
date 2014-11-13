#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include "tree.h"

enum type {
    N_ATOMIC = 1,
    N_PRIMARY_EXPRESSION,
    N_NEGATION,
    N_MULTIPLICATION,
    N_MULTIPLICATIVE_EXPRESSION,
    N_ADDITION,
    N_ADDITIVE_EXPRESSION,
    N_EXPRESSION,
    N_COMPONENTS,
    N_VECTOR,
    N_DECLARATION,
    N_DECLARATION_SEQUENCE,
    N_TRANSLATION_UNIT
};

extern char *AST_TYPE_NAMES[];

enum alternative {
    ALT_ATOMIC = 1,
    ALT_VECTOR_ATOMIC,
    ALT_PRIMARY_EXPRESSION,
    ALT_NEGATION,
    ALT_MULTIPLICATION,
    ALT_MULTIPLICATIVE_EXPRESSION,
    ALT_ADDITION,
    ALT_ADDITIVE_EXPRESSION,
    ALT_EXPRESSION,
    ALT_COMPONENTS,
    ALT_VECTOR,
    ALT_DECLARATION,
    ALT_DECLARATION_SEQUENCE,
    ALT_TRANSLATION_UNIT,

    ALT_IDENTIFIER,
    ALT_NUMBER,

    ALT_ADD,
    ALT_SUB,
    ALT_MULT,
    ALT_DIV,
};

struct payload {
	enum type type;
	enum alternative alternative;

    union {
        struct {
            union {
                double number;
                const char *identifier;
            };
        } atomic;

        struct {
            const char *identifier;
        } vector_atomic;

        struct {
            const char *identifier;
        } declaration;
    };
};

void payload_free(void *payload);

#endif
