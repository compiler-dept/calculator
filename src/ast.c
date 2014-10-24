#include "ast.h"

/*struct node *node_factory(enum type type, enum alternative alternative)
{
	struct node *node = malloc(sizeof(struct node));
	node->type = type;

	switch(type)
	{
		case N_ATOMIC:
			node->atomic = malloc(sizeof(struct atomic));
			node->atomic->alternative = alternative;
			break;
		case N_PRIMARY_EXPRESSION:
			node->primary_expression = malloc(sizeof(struct primary_expression));
			node->primary_expression->alternative = alternative;
			break;
		case N_NEGATION:
			node->negation = malloc(sizeof(struct negation));
			node->negation->alternative = alternative;
			break;
		case N_MULTIPLICATION:
			node->multiplication = malloc(sizeof(struct multiplication));
			node->multiplication->alternative = alternative;
			break;
		case N_MULTIPLICATIVE_EXPRESSION:
			node->multiplicative_expression = malloc(sizeof(struct multiplicative_expression));
			node->multiplicative_expression->alternative = alternative;
			break;
		case N_ADDITION:
			node->addition = malloc(sizeof(struct addition));
			node->addition->alternative = alternative;
			break;
		case N_ADDITIVE_EXPRESSION:
			node->additive_expression = malloc(sizeof(struct additive_expression));
			node->additive_expression->alternative = alternative;
			break;
		case N_ATOMIC_EXPRESSION:
			node->atomic_expression = malloc(sizeof(struct atomic_expression));
			node->atomic_expression->alternative = alternative;
			break;
		case N_COMPONENTS:
			node->atomic = malloc(sizeof(struct atomic));
			node->atomic->alternative = alternative;
			break;
		case N_VECTOR:
			node->atomic = malloc(sizeof(struct atomic));
			node->atomic->alternative = alternative;
			break;
		case N_VECTOR_PROMARY_EXPRESSION:
			node->atomic = malloc(sizeof(struct atomic));
			node->atomic->alternative = alternative;
			break;
		case N_VECTOR_NEGATION:
			node->atomic = malloc(sizeof(struct atomic));
			node->atomic->alternative = alternative;
			break;
		case N_VECTOR_SCALAR_MULTIPLICATION:
			node->atomic = malloc(sizeof(struct atomic));
			node->atomic->alternative = alternative;
			break;
		case N_VECTOR_ADDITION:
			node->atomic = malloc(sizeof(struct atomic));
			node->atomic->alternative = alternative;
			break;
		case N_VECTOR_EXPRESSION:
			node->atomic = malloc(sizeof(struct atomic));
			node->atomic->alternative = alternative;
			break;
		case N_SCALAR_DECLARATION:
			node->scalar_declaration = malloc(sizeof(struct scalar_declaration));
			node->scalar_declaration->alternative = alternative;
			break;
		case N_VECTOR_DECLARATION:
			node->atomic = malloc(sizeof(struct atomic));
			node->atomic->alternative = alternative;
			break;
		case N_DECLARATION:
			node->declaration = malloc(sizeof(struct declaration));
			node->declaration->alternative = alternative;
			break;
		case N_DECLARATION_SEQUENCE:
			node->declaration_sequence = malloc(sizeof(struct declaration_sequence));
			node->declaration_sequence->alternative = alternative;
			break;
		case N_TRANSLATION_UNIT:
			node->translation_unit = malloc(sizeof(struct translation_unit));
			node->translation_unit->alternative = alternative;
			break;
		default:
			// will never happen
	}

	return node;
}*/

struct ast_iterator *ast_iterator_init(struct node *node)
{
	struct ast_iterator *iterator = malloc(sizeof(struct ast_iterator));
	iterator->current = node;
	iterator->stack = NULL;

	return iterator;
};

void *ast_iterator_next(struct ast_iterator *iterator)
{
	while(iterator->current->type != N_ATOMIC)
	{
		stack_push(&(iterator->stack), iterator->current);

		switch(iterator->current->alternative)
		{
			case ALT_ATOMIC:
				itarator->current = iterator->current->atomic;
				break;
			case ALT_PRIMARY_EXPRESSION:
				itarator->current = iterator->current->atomic_expression;
				break;
			case ALT_NEGATION:
				itarator->current = iterator->current->negation;
				break;
			case ALT_MULTIPLICATION:
				itarator->current = iterator->current->multiplication;
				break;
			case ALT_MULTIPLICATIVE_EXPRESSION:
				itarator->current = iterator->current->multiplicative_expression;
				break;
			case ALT_ADDITION:
				itarator->current = iterator->current->addition;
				break;
			case ALT_ADDITIVE_EXPRESSION:
				itarator->current = iterator->current->additive_expression;
				break;
			case ALT_ATOMIC_EXPRESSION:
				itarator->current = iterator->current->atomic_expression;
				break;
			/*case ALT_COMPONENTS:
				itarator->current = iterator->current->
				break;
			case ALT_VECTOR:
				itarator->current = iterator->current->
				break;
			case ALT_VECTOR_PROMARY_EXPRESSION:
				itarator->current = iterator->current->
				break;
			case ALT_VECTOR_NEGATION:
				itarator->current = iterator->current->
				break;
			case: ALT_VECTOR_SCALAR_MULTIPLICATION:
				itarator->current = iterator->current->
				break;
			case ALT_VECTOR_ADDITION:
				itarator->current = iterator->current->
				break;
			case ALT_VECTOR_EXPRESSION:
				itarator->current = iterator->current->
				break;*/
			case ALT_SCALAR_DECLARATION:
				itarator->current = iterator->current->scalar_declaration;
				break;
			/*case ALT_VECTOR_DECLARATION:
				itarator->current = iterator->current->
				break;*/
			case ALT_DECLARATION:
				itarator->current = iterator->current->declaration;
				break;
			case ALT_DECLARATION_SEQUENCE:
				itarator->current = iterator->current->declaration_sequence;
				break;
			case ALT_TRANSLATION_UNIT:
				// will never happen
				break;
			case ALT_IDENTIFIER:
				itarator->current = iterator->current->identifier;
				break;
			case ALT_NUMBER:
				itarator->current = &(iterator->current->number;) // Check this
				break;
			case ALT_ADD:
				itarator->current = iterator->current->expression0;
				break;
			case ALT_SUB:
				itarator->current = iterator->current->expression0;
				break;
			case ALT_MULT:
				itarator->current = iterator->current->negation0;
				break;
			case ALT_DIV:
				itarator->current = iterator->current->negation0;
				break;
			default:
				// will never happen
		}

		if(iterator->current.alternative == ALT_ADD || iterator->current.alternative == ALT_SUB)
		{
			iterator->current = iterator->current->expression1;
		}

		if(iterator->current.alternative == ALT_MULT || iterator->current.alternative == ALT_DIV)
		{
			iterator->current = iterator->current->negation1;
		}

		return stack_pop(&(iterator->stack));
	}
};
