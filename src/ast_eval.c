#include "ast_eval.h"
#include "gram.h"
#include "parser_state.h"
#include "../tests/test_helper.h"

double ev_atomic_expression(struct atomic_expression *, struct hashmap **);

double ev_atomic(struct atomic *node, struct hashmap **mappings)
{
	double value;
	if (node->alternative == ALT_NUMBER) {
		value = node->number;
	} else {
		value = *((double *)hashmap_get(*mappings, node->identifier));
	}
	return value;
}

double ev_primary_expression(struct primary_expression *node,
			     struct hashmap **mappings)
{
	double value;
	if (node->alternative == ALT_ATOMIC_EXPRESSION) {
		value = ev_atomic_expression(node->atomic_expression, mappings);
	} else {
		value = ev_atomic(node->atomic, mappings);
	}
	return value;
}

double ev_negation(struct negation *node, struct hashmap **mappings)
{
	double value;
	if (node->alternative == ALT_NEGATION) {
		value = ev_negation(node->negation, mappings);
	} else {
		value =
		    ev_primary_expression(node->primary_expression, mappings);
	}
	return value;
}

double ev_multiplication(struct multiplication *node, struct hashmap **mappings)
{
	double value;
	double op0 = ev_negation(node->negation0, mappings);
	double op1 = ev_negation(node->negation1, mappings);
	if (node->alternative == ALT_MULT) {
		value = op0 * op1;
	} else {
		value = op0 / op1;
	}
	return value;
}

double ev_multiplicative_expression(struct multiplicative_expression *node,
				    struct hashmap **mappings)
{
	double value;
	if (node->alternative == ALT_MULTIPLICATION) {
		value = ev_multiplication(node->multiplication, mappings);
	} else {
		value = ev_negation(node->negation, mappings);
	}
	return value;
}

double ev_addition(struct addition *node, struct hashmap **mappings)
{
	double value;
	double op0 = ev_multiplicative_expression(node->expression0, mappings);
	double op1 = ev_multiplicative_expression(node->expression1, mappings);
	if (node->alternative == ALT_ADD) {
		value = op0 + op1;
	} else {
		value = op0 - op1;
	}
	return value;
}

double ev_additive_expression(struct additive_expression *node,
			      struct hashmap **mappings)
{
	double value;
	if (node->alternative == ALT_ADDITION) {
		value = ev_addition(node->addition, mappings);
	} else {
		value =
		    ev_multiplicative_expression
		    (node->multiplicative_expression, mappings);
	}
	return value;
}

double ev_atomic_expression(struct atomic_expression *node,
			    struct hashmap **mappings)
{
	return ev_additive_expression(node->additive_expression, mappings);
}

void ev_scalar_declaration(struct scalar_declaration *node,
			   struct hashmap **mappings)
{
	double *value = malloc(sizeof(double));
	*value = ev_atomic_expression(node->atomic_expression, mappings);
	hashmap_put(mappings, node->identifier, value);
}

void ev_declaration(struct declaration *node, struct hashmap **mappings)
{
	if (node->alternative == ALT_SCALAR_DECLARATION) {
		ev_scalar_declaration(node->scalar_declaration, mappings);
	}
}

void ev_declaration_sequence(struct declaration_sequence *node,
			     struct hashmap **mappings)
{
	ev_declaration(node->declaration, mappings);
	if (node->declaration_sequence) {
		ev_declaration_sequence(node->declaration_sequence, mappings);
	}
}

void ev_translation_unit(struct translation_unit *node,
			 struct hashmap **mappings)
{
	ev_declaration_sequence(node->declaration_sequence, mappings);
}
