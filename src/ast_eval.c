#include "ast_eval.h"

void ast_eval(struct node *root, struct hashmap **mappings)
{
    struct ast_iterator *it = ast_iterator_init(root, POSTORDER);
    struct stack *stack = NULL;

    struct node *temp = NULL;

    double *val0, *val1, *val2;
    struct ast_eval_result *data0, *data1, *data2;

    while ((temp = ast_iterator_next(it))) {
        switch (temp->type) {
        case N_ATOMIC:
            if (temp->alternative == ALT_NUMBER) {
                val0 = malloc(sizeof(double));
                *val0 = temp->payload.atomic.number;
                stack_push(&stack, val0);
            } else if (temp->alternative == ALT_IDENTIFIER) {
                data0 = hashmap_get(*mappings,
                                   temp->payload.
                                   atomic.identifier);
                if (data0->alternative == ALT_NUMBER) {
                    val1 = malloc(sizeof(double));
                    *val1 = data0->number;
                    stack_push(&stack, val1);
                } else if (data0->alternative == ALT_VECTOR) {
                    stack_push(&stack, data0);
                }
            }
            break;
        case N_VECTOR_ATOMIC:
            if (temp->alternative == ALT_IDENTIFIER) {
                data0 = hashmap_get(*mappings,
                                   temp->payload.
                                   vector_atomic.identifier);
                stack_push(&stack, data0);
            }
            break;
        case N_NEGATION:
            if (temp->alternative == ALT_NEGATION) {
                val0 = stack_pop(&stack);
                *val0 *= -1;
                stack_push(&stack, val0);
            }
            break;
        case N_VECTOR_NEGATION:
            if (temp->alternative == ALT_VECTOR_NEGATION) {
                data0 = stack_pop(&stack);
                for (int i = 0; i < data0->vector.compc; i++) {
                    data0->vector.compv[i] *= -1;
                }
                stack_push(&stack, data0);
            }
            break;
        case N_ADDITION:
            val1 = stack_pop(&stack);	// left operand was pushed first
            val0 = stack_pop(&stack);	// so it needs to be retreived second
            if (temp->alternative == ALT_SUB) {
                *val1 *= -1;
            }

            val2 = malloc(sizeof(double));
            *val2 = *val0 + *val1;
            stack_push(&stack, val2);

            free(val0);
            free(val1);
            break;
        case N_VECTOR_ADDITION:
            if (temp->alternative == ALT_VECADD) {
                data0 = stack_pop(&stack);
                data1 = stack_pop(&stack);
                data2 = malloc(sizeof(struct ast_eval_result *) + data0->vector.compc * sizeof(double));
                data2->vector.compc = data0->vector.compc;

                for (int i = 0; i < data1->vector.compc; i++) {
                    data2->vector.compv[i] = data0->vector.compv[i] + data1->vector.compv[i];
                }

                stack_push(&stack, data2);
                free(data0);
                free(data1);
            }
            break;
        case N_MULTIPLICATION:
            val1 = stack_pop(&stack);	// left operand was pushed first
            val0 = stack_pop(&stack);	// so it needs to be retreived second
            if (temp->alternative == ALT_DIV) {
                *val1 = 1 / *val1;
            }
            val2 = malloc(sizeof(double));
            *val2 = *val0 ** val1;
            stack_push(&stack, val2);
            free(val0);
            free(val1);
            break;
        case N_VECTOR_SCALAR_MULTIPLICATION:
            if (temp->alternative == ALT_SCMULT) {
                data0 = stack_pop(&stack);
                data1 = malloc(sizeof(struct ast_eval_result *) + data0->vector.compc * sizeof(double));
                data1->vector.compc = data0->vector.compc;
                val0 = stack_pop(&stack);

                for (int i = 0; i < data0->vector.compc; i++) {
                    data1->vector.compv[i] = data0->vector.compv[i] * (*val0);
                }

                stack_push(&stack, data1);
                free(data0);
            }
            break;
        case N_SCALAR_DECLARATION:
            val0 = stack_pop(&stack);
            data0 = malloc(sizeof(struct ast_eval_result));
            data0->alternative = ALT_NUMBER;
            data0->number = *val0;
            free(hashmap_put
                 (mappings,
                  temp->payload.scalar_declaration.identifier,
                  data0));
            free(val0);
            break;
        case N_VECTOR:
            data0 =
                malloc(sizeof(struct ast_eval_result) +
                       temp->childc * sizeof(double));
            data0->alternative = ALT_VECTOR;
            data0->vector.compc = temp->childc;
            for (int i = 0; i < temp->childc; i++) {
                val0 = stack_pop(&stack);
                data0->vector.compv[i] = *val0;
                free(val0);
            }
            stack_push(&stack, data0);
            break;
        case N_VECTOR_DECLARATION:
            data0 = stack_pop(&stack);
            free(hashmap_put(mappings,
                             temp->payload.
                             vector_declaration.identifier, data0));
            break;
        }
    }

    while ((temp = stack_pop(&stack))) {
        free(temp);
    }

    free(it);
}
