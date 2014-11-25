#include "stack.h"
#include "tree.h"
#include "ast_eval.h"

struct ast_eval_result *alloc_number(double val)
{
    struct ast_eval_result *data;
    data = malloc(sizeof(struct ast_eval_result));
    data->alternative = ALT_NUMBER;
    data->number = val;
    return data;
}

struct ast_eval_result *alloc_vector(int size)
{
    struct ast_eval_result *data;
    data = malloc(sizeof(struct ast_eval_result) + size * sizeof(double));
    data->alternative = ALT_VECTOR;
    data->vector.compc = size;
    return data;
}

struct ast_eval_result *copy(struct ast_eval_result *data)
{
    struct ast_eval_result *data1 = NULL;
    if (data->alternative == ALT_NUMBER) {
        data1 = malloc(sizeof(struct ast_eval_result));
        data1->alternative = ALT_NUMBER;
        data1->number = data->number;
    } else if (data->alternative == ALT_VECTOR) {
        data1 =
            malloc(sizeof
                   (struct ast_eval_result) +
                   data->vector.compc * sizeof(double));
        data1->alternative = ALT_VECTOR;
        data1->vector.compc = data->vector.compc;
        for (int i = 0; i < data->vector.compc; i++) {
            data1->vector.compv[i] = data->vector.compv[i];
        }
    }
    return data1;
}

void inverse_additive(struct ast_eval_result *data)
{
    if (data->alternative == ALT_NUMBER) {
        data->number *= -1;
    } else if (data->alternative == ALT_VECTOR) {
        for (int i = 0; i < data->vector.compc; i++) {
            data->vector.compv[i] *= -1;
        }
    }
}

void inverse_multiplicative(struct ast_eval_result *data)
{
    if (data->alternative == ALT_NUMBER) {
        data->number = 1 / data->number;
    } else if (data->alternative == ALT_VECTOR) {
        for (int i = 0; i < data->vector.compc; i++) {
            data->vector.compv[i] = 1 / data->vector.compv[i];
        }
    }
}

void ast_eval(struct node *root, struct hashmap **mappings)
{
    struct tree_iterator *it = tree_iterator_init(&root, POSTORDER);
    struct stack *stack = NULL;

    struct node *temp = NULL;

    struct ast_eval_result *data0, *data1, *data2;

    char abort = 0;

    while ((temp = tree_iterator_next(it))) {
        switch (((struct payload *)(temp->payload))->type) {
        case N_ATOMIC:
            if (((struct payload *)(temp->payload))->alternative == ALT_NUMBER) {
                data0 =
                    alloc_number(((struct payload *)(temp->payload))->atomic.number);
                stack_push(&stack, data0);
            } else if (((struct payload *)(temp->payload))->alternative == ALT_IDENTIFIER) {
                const char *id = ((struct payload *)
                        (temp->payload))->atomic.identifier;
                data0 = hashmap_get(*mappings, id);
                if (data0) {
                    data1 = copy(data0);
                    stack_push(&stack, data1);
                } else {
                    printf("Use of Undefined Variable: %s\n", id); 
                    abort = 1;
                    break ;
                }
            }
            break;
        case N_VECTOR:
            data0 = alloc_vector(temp->childc);
            for (int i = 0; i < temp->childc; i++) {
                data1 = stack_pop(&stack);
                if (data1->alternative != ALT_NUMBER){
                    printf("Undefined Operation: Multidimensional Vector\n");
                    free(data1);
                    abort = 1;
                    break;
                }
                data0->vector.compv[i] = data1->number;
                free(data1);
            }
            stack_push(&stack, data0);
            break;
        case N_NEGATION:
            if (((struct payload *)(temp->payload))->alternative == ALT_NEGATION) {
                data0 = stack_pop(&stack);
                inverse_additive(data0);
                stack_push(&stack, data0);
            }
            break;
        case N_ADDITION:
            data1 = stack_pop(&stack);	// left operand was pushed first
            data0 = stack_pop(&stack);	// so it needs to be retreived second
            if (((struct payload *)(temp->payload))->alternative == ALT_SUB) {
                inverse_additive(data1);
            }

            if (data0->alternative == ALT_NUMBER
                && data1->alternative == ALT_NUMBER) {
                data2 =
                    alloc_number(data0->number + data1->number);
            } else if (data0->alternative == ALT_VECTOR
                       && data1->alternative == ALT_VECTOR) {
                if (data0->vector.compc != data1->vector.compc){
                    printf("Undefined Operation: Addition of Vectors of Different Lengths.\n");
                    abort = 1;
                    free(data0);
                    free(data1);
                    break;
                }

                data2 = alloc_vector(data0->vector.compc);

                for (int i = 0; i < data0->vector.compc; i++) {
                    data2->vector.compv[i] =
                        data0->vector.compv[i] +
                        data1->vector.compv[i];
                }
            } else {
                printf("Undefined Operation: Mixed Scalar Vector Addition\n");
                free(data0);
                free(data1);
                abort = 1;
                break;
            }
            stack_push(&stack, data2);

            free(data0);
            free(data1);
            break;
        case N_MULTIPLICATION:
            data1 = stack_pop(&stack);	// left operand was pushed first
            data0 = stack_pop(&stack);	// so it needs to be retreived second
            if (((struct payload *)(temp->payload))->alternative == ALT_DIV) {
                inverse_multiplicative(data1);
            }
            if (data0->alternative == ALT_NUMBER) {
                if (data1->alternative == ALT_NUMBER) {
                    data2 =
                        alloc_number(data0->number *
                                     data1->number);
                } else if (data1->alternative == ALT_VECTOR) {
                    data2 =
                        alloc_vector(data1->vector.compc);
                    for (int i = 0; i < data1->vector.compc;
                         i++) {
                        data2->vector.compv[i] =
                            data1->vector.compv[i] *
                            (data0->number);
                    }
                }
            } else {
                printf("Undefined Operation: Multiplication 1. Operand a Vector\n");
                free(data0);
                free(data1);
                abort = 1;
                break;
            }
            stack_push(&stack, data2);
            free(data0);
            free(data1);
            break;
        case N_DECLARATION:
            data0 = stack_pop(&stack);
            if (data0->alternative == ALT_NUMBER) {
                data1 = copy(data0);
                free(hashmap_put
                     (mappings,
                      ((struct payload *)(temp->payload))->declaration.identifier,
                      data1));
            } else if (data0->alternative == ALT_VECTOR) {
                data1 = copy(data0);
                free(hashmap_put(mappings,
                                 ((struct payload *)(temp->payload))->declaration.
                                 identifier, data1));
            }
            free(data0);
            break;
        }
        if (abort){
            break;
        }
    }


    while ((data0 = stack_pop(&stack))) {
        free(data0);
    }

    tree_iterator_free(it);
}
