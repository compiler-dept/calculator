#include "ast_eval.h"

void inverse_additive(struct ast_eval_result *data)
{
	if (data->alternative == ALT_NUMBER) {
		data->number *= -1;
	} else if (data->alternative == ALT_VECTOR) {
		for (int i = 0; i < data0->vector.compc; i++) {
			data0->vector.compv[i] *= -1;
		}
	}
}

void inverse_multiplicative(struct ast_eval_result *data)
{
	if (data->alternative == ALT_NUMBER) {
		data->number = 1 / data->number;
	} else if (data->alternative == ALT_VECTOR) {
		for (int i = 0; i < data0->vector.compc; i++) {
			data0->vector.compv[i] = 1 / data0->vector.compv[i];
		}
	}
}

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
				data0 = malloc(sizeof(ast_eval_result));
				data0->alternative = ALT_NUMBER;
				data0->number = temp->payload.atomic.number;
				stack_push(&stack, val0);
			} else if (temp->alternative == ALT_IDENTIFIER) {
				data0 = hashmap_get(*mappings,
						    temp->payload.
						    atomic.identifier);
				if (data0->alternative == ALT_NUMBER) {
					val1 = malloc(sizeof(double));
					*val1 = data0->number;
					stack_push(&stack, val1);
				} else if (data0->alternative = ALT_VECTOR) {
					data1 =
					    malloc(sizeof
						   (struct ast_eval_result) +
						   data0->vector.compc *
						   sizeof(double));
					data1->alternative = ALT_VECTOR;
					data1->vector.compc =
					    data0->vector.compc;
					for (int i = 0; i < data0->vector.compc;
					     i++) {
						data1->vector.compv[i] =
						    data0->vector.compv[i];
					}
					stack_push(&stack, data1);
				}
			}
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
		case N_NEGATION:
			if (temp->alternative == ALT_NEGATION) {
				data0 = stack_pop(&stack);
				inverse_additive(data0);
				stack_push(&stack, data0);
			}
			break;
		case N_ADDITION:
			data1 = stack_pop(&stack);	// left operand was pushed first
			data0 = stack_pop(&stack);	// so it needs to be retreived second
			if (temp->alternative == ALT_SUB) {
				inverse_additive(data1);
			}

			data2 = malloc(sizeof(struct ast_eval_result));
			if (data0->alternative == ALT_NUMBER
			    && data1->alternative == ALT_NUMBER) {
				data2->alternative = ALT_NUMBER;
				data2->number = data0->number + data1->number;
			} else if (data0->alternative == ALT_VECTOR
				   && data2->alternative == ALT_VECTOR) {
				data2 =
				    malloc(sizeof(struct ast_eval_result) +
					   data0->vector.compc *
					   sizeof(double));
				data2->alternative = ALT_VECTOR;
				data2->vector.compc = data0->vector.compc;

				for (int i = 0; i < data0->vector.compc; i++) {
					data2->vector.compv[i] =
					    data0->vector.compv[i] +
					    data1->vector.compv[i];
				}
			}
			stack_push(&stack, data2);

			free(data0);
			free(data1);
			break;
		case N_MULTIPLICATION:
			data1 = stack_pop(&stack);	// left operand was pushed first
			data0 = stack_pop(&stack);	// so it needs to be retreived second
            if (temp->alternative == ALT_DIV) {
                inverse_multiplicative(data1);
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
				val0 = stack_pop(&stack);

				data1 =
				    malloc(sizeof(struct ast_eval_result) +
					   data0->vector.compc *
					   sizeof(double));
				data1->alternative = ALT_VECTOR;
				data1->vector.compc = data0->vector.compc;

				for (int i = 0; i < data0->vector.compc; i++) {
					data1->vector.compv[i] =
					    data0->vector.compv[i] * (*val0);
				}

				stack_push(&stack, data1);
				free(data0);
				free(val0);
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
		case N_VECTOR_DECLARATION:
			data0 = stack_pop(&stack);
			data1 =
			    malloc(sizeof(struct ast_eval_result) +
				   data0->vector.compc * sizeof(double));
			data1->alternative = ALT_VECTOR;
			data1->vector.compc = data0->vector.compc;
			for (int i = 0; i < data0->vector.compc; i++) {
				data1->vector.compv[i] = data0->vector.compv[i];
			}
			free(hashmap_put(mappings,
					 temp->payload.
					 vector_declaration.identifier, data1));
			free(data0);
			break;
		}
	}

	while ((data0 = stack_pop(&stack))) {
		free(data0);
	}

	free(it);
}
