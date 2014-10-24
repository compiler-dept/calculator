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
	struct node *node = NULL;

	while(iterator->current != NULL)
	{
		stack_push(&(iterator->stack), iterator->current);

		if(iterator->current->type == N_ATOMIC)
		{
			iterator->current = NULL;
		}
		else
		{
			iterator->current = iterator->current->childv[0];
		}
	}

	node = stack_pop(&(iterator->stack));
	if(node->type == N_MULTIPLICATION
		|| node->type == N_ADDITION
		|| node->type == N_DECLARATION_SEQUENCE)
	{
		iterator->current = node->childv[1];
	}

	return node;
};

/*
if(iterator->current.alternative == ALT_ADD || iterator->current.alternative == ALT_SUB)
{
	iterator->current = iterator->current->expression1;
}

if(iterator->current.alternative == ALT_MULT || iterator->current.alternative == ALT_DIV)
{
	iterator->current = iterator->current->negation1;
}

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
case ALT_COMPONENTS:
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
	break;
case ALT_SCALAR_DECLARATION:
	itarator->current = iterator->current->scalar_declaration;
	break;
case ALT_VECTOR_DECLARATION:
	itarator->current = iterator->current->
	break;
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
*/
