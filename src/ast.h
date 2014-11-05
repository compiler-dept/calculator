#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include "stack.h"
#include "hashmap.h"

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
    N_VECTOR_PRIMARY_EXPRESSION,
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

extern char *AST_TYPE_NAMES[];

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

struct payload {
    union {
        struct {
            union {
                double number;
                const char *identifier;
            };
        } atomic;

        struct {
            const char *identifier;
        } scalar_declaration;

        struct {
            const char *identifier;
        } vector_declaration;
    };
};

struct node {
    enum type type;
    enum alternative alternative;
    struct payload payload;
    int childc;
    struct node **childv;
};

/**
 * AST iterator
 */
enum iterator_type {
    PREORDER = 1,
    POSTORDER,
    INORDER
};

struct ast_iterator {
    enum iterator_type type;
    struct stack *stack;
    struct node *current;
};

struct ast_iterator *ast_iterator_init(struct node *node, enum iterator_type type);
struct node *ast_iterator_next(struct ast_iterator *iterator);
void ast_free(struct node *node);

#endif
