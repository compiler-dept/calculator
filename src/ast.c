#include "ast.h"

struct ast_iterator *ast_iterator_init(struct node *node)
{
	struct ast_iterator *iterator = malloc(sizeof(struct ast_iterator));
	iterator->current = node;
	iterator->stack = NULL;

	return iterator;
};

struct node *ast_iterator_next(struct ast_iterator *iterator)
{
	while (iterator->current->type != N_ATOMIC) {
		stack_push(&(iterator->stack), iterator->current);

		switch (iterator->current->alternative) {
		case ALT_ATOMIC:
			iterator->current = iterator->current->atomic;
			break;
		case ALT_PRIMARY_EXPRESSION:
			iterator->current =
			    iterator->current->atomic_expression;
			break;
		case ALT_NEGATION:
			iterator->current = iterator->current->negation;
			break;
		case ALT_MULTIPLICATION:
			iterator->current = iterator->current->multiplication;
			break;
		case ALT_MULTIPLICATIVE_EXPRESSION:
			iterator->current = iterator->current->break;
		case ALT_ADDITION:
			iterator->current = iterator->current->break;
		case ALT_ADDITIVE_EXPRESSION:
			iterator->current = iterator->current->break;
		case ALT_ATOMIC_EXPRESSION:
			iterator->current = iterator->current->break;
		case ALT_COMPONENTS:
			iterator->current = iterator->current->break;
		case ALT_VECTOR:
			iterator->current = iterator->current->break;
		case ALT_VECTOR_PROMARY_EXPRESSION:
			iterator->current = iterator->current->break;
		case ALT_VECTOR_NEGATION:
			iterator->current = iterator->current->break;
		case:
 ALT_VECTOR_SCALAR_MULTIPLICATION:
			iterator->current = iterator->current->break;
		case ALT_VECTOR_ADDITION:
			iterator->current = iterator->current->break;
		case ALT_VECTOR_EXPRESSION:
			iterator->current = iterator->current->break;
		case ALT_SCALAR_DECLARATION:
			iterator->current = iterator->current->break;
		case ALT_VECTOR_DECLARATION:
			iterator->current = iterator->current->break;
		case ALT_DECLARATION:
			iterator->current = iterator->current->break;
		case ALT_DECLARATION_SEQUENCE:
			iterator->current = iterator->current->break;
		case ALT_TRANSLATION_UNIT:
			iterator->current = iterator->current->break;
		case ALT_IDENTIFIER:
			iterator->current = iterator->current->break;
		case ALT_NUMBER:
			iterator->current = iterator->current->break;
		case ALT_ADD:
			iterator->current = iterator->current->break;
		case ALT_SUB:
			iterator->current = iterator->current->break;
		case ALT_MULT:
			iterator->current = iterator->current->break;
		case ALT_DIV:
			iterator->current = iterator->current->break;
		default:
			// will never happen
		}
	}
};
