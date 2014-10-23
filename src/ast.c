#include "ast.h"

struct ast_iterator *ast_iterator_init(struct translation_unit
				       *translation_unit)
{
	struct ast_iterator *iterator = malloc(sizeof(struct ast_iterator));
	itearator->current = translation_unit;
	iterator->stack = NULL;

	return iterator;
};

void *ast_iterator_next(struct ast_iterator *iterator)
{
	while (current->type != N_ATOMIC) {
		stack_push(&(iterator->stack), iterator->current);

		switch (iterator->current->alternative) {
		case ALT_ATOMIC:
			itarator->current = iterator->current->atomic;
			break;
		case ALT_PRIMARY_EXPRESSION:
			itarator->current =
			    iterator->current->atomic_expression;
			break;
		case ALT_NEGATION:
			itarator->current = iterator->current->negation;
			break;
		case ALT_MULTIPLICATION:
			itarator->current = iterator->current->multiplication;
			break;
		case ALT_MULTIPLICATIVE_EXPRESSION:
			itarator->current = iterator->current->break;
		case ALT_ADDITION:
			itarator->current = iterator->current->break;
		case ALT_ADDITIVE_EXPRESSION:
			itarator->current = iterator->current->break;
		case ALT_ATOMIC_EXPRESSION:
			itarator->current = iterator->current->break;
		case ALT_COMPONENTS:
			itarator->current = iterator->current->break;
		case ALT_VECTOR:
			itarator->current = iterator->current->break;
		case ALT_VECTOR_PROMARY_EXPRESSION:
			itarator->current = iterator->current->break;
		case ALT_VECTOR_NEGATION:
			itarator->current = iterator->current->break;
		case:
 ALT_VECTOR_SCALAR_MULTIPLICATION:
			itarator->current = iterator->current->break;
		case ALT_VECTOR_ADDITION:
			itarator->current = iterator->current->break;
		case ALT_VECTOR_EXPRESSION:
			itarator->current = iterator->current->break;
		case ALT_SCALAR_DECLARATION:
			itarator->current = iterator->current->break;
		case ALT_VECTOR_DECLARATION:
			itarator->current = iterator->current->break;
		case ALT_DECLARATION:
			itarator->current = iterator->current->break;
		case ALT_DECLARATION_SEQUENCE:
			itarator->current = iterator->current->break;
		case ALT_TRANSLATION_UNIT:
			itarator->current = iterator->current->break;
		case ALT_IDENTIFIER:
			itarator->current = iterator->current->break;
		case ALT_NUMBER:
			itarator->current = iterator->current->break;
		case ALT_ADD:
			itarator->current = iterator->current->break;
		case ALT_SUB:
			itarator->current = iterator->current->break;
		case ALT_MULT:
			itarator->current = iterator->current->break;
		case ALT_DIV:
			itarator->current = iterator->current->break;
		default:
			// will never happen
		}
	}
};
