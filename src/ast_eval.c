#include "ast_eval.h"

void ast_free(struct node *root, struct hashmap **mappings)
{
	struct ast_iterator *it = ast_iterator_init(root, POSTORDER);
	struct stack *stack = NULL;

	struct node *temp = NULL;

	while ((temp = ast_iterator_next(it))) {

	}
}

void ast_eval(struct node *root, struct hashmap **mappings)
{
	struct ast_iterator *it = ast_iterator_init(root, POSTORDER);
	struct stack *stack = NULL;

	struct node *temp = NULL;

	double *val0, *val1, *val2;

	while ((temp = ast_iterator_next(it))) {
		switch (temp->type) {
		case N_ATOMIC:
			if (temp->alternative == ALT_NUMBER) {
				val0 = malloc(sizeof(double));
				*val0 = temp->payload.atomic.number;
				stack_push(&stack, val0);
			} else if (temp->alternative == ALT_IDENTIFIER) {
				val0 = hashmap_get(*mappings,
						   temp->payload.
						   atomic.identifier);
                val1 = malloc(sizeof(double));
                *val1 = *val0;
				stack_push(&stack, val1);
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
			*val2 = *val0 * *val1;
			stack_push(&stack, val2);
			free(val0);
			free(val1);
			break;
		case N_SCALAR_DECLARATION:
			val0 = stack_pop(&stack);
            val1 = malloc(sizeof(double));
            *val1 = *val0;
			hashmap_put(mappings,
				    temp->payload.scalar_declaration.identifier,
				    val1);
            free(val0);

			break;
		}
	}

    free(it);
}
