#include "ast_eval.h"

void ast_eval(struct node *root, struct hashmap **mappings)
{
    struct ast_iterator *it = ast_iterator_init(root, POSTORDER);
    struct stack *stack = NULL;

    struct node *temp = NULL;

    double *val0, *val1, *val2;
    struct ast_eval_result *data;

    while ((temp = ast_iterator_next(it))) {
        switch (temp->type) {
        case N_ATOMIC:
            if (temp->alternative == ALT_NUMBER) {
                val0 = malloc(sizeof(double));
                *val0 = temp->payload.atomic.number;
                stack_push(&stack, val0);
            } else if (temp->alternative == ALT_IDENTIFIER) {
                data = hashmap_get(*mappings,
                                   temp->payload.
                                   atomic.identifier);
                if (data->alternative == ALT_NUMBER) {
                    val1 = malloc(sizeof(double));
                    *val1 = data->number;
                    stack_push(&stack, val1);
                } else if (data->alternative == ALT_VECTOR) {
                    stack_push(&stack, data);
                }
            }
            break;
        case N_NEGATION:
            if (temp->alternative == ALT_NEGATION) {
                val0 = stack_pop(&stack);
                *val0 *= -1;
                stack_push(&stack, val0);
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
        case N_SCALAR_DECLARATION:
            val0 = stack_pop(&stack);
            data = malloc(sizeof(struct ast_eval_result));
            data->alternative = ALT_NUMBER;
            data->number = *val0;
            free(hashmap_put
                 (mappings,
                  temp->payload.scalar_declaration.identifier,
                  data));
            free(val0);
            break;
        case N_VECTOR:
            data =
                malloc(sizeof(struct ast_eval_result) +
                       temp->childc * sizeof(double));
            data->alternative = ALT_VECTOR;
            data->vector.compc = temp->childc;
            for (int i = 0; i < temp->childc; i++) {
                val0 = stack_pop(&stack);
                data->vector.compv[i] = *val0;
                free(val0);
            }
            stack_push(&stack, data);
            break;
        case N_VECTOR_DECLARATION:
            data = stack_pop(&stack);
            free(hashmap_put(mappings,
                             temp->payload.
                             vector_declaration.identifier, data));
            break;
        }
    }

    while ((temp = stack_pop(&stack))) {
        free(temp);
    }

    free(it);
}
