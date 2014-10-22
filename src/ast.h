#ifndef AST_H
#define AST_H

enum type {
	N_ATOMIC = 1,
	N_PRIMARY_EXPRESSION,
	N_NEGATION,
	N_MULTIPLICATION,
	N_MULTIPLICATIVE_EXPRESSION,
	N_ADDITION,
	N_ADDITIVE_EXPRESSION,
	N_ATOMIC_EXPRESSION,
	N_COMPONENTS,
	N_VECTOR,
	N_VECTOR_PROMARY_EXPRESSION,
	N_VECTOR_NEGATION,
	N_VECTOR_SCALAR_MULTIPLICATION,
	N_VECTOR_ADDITION,
	N_VECTOR_EXPRESSION,
	N_SCALAR_DECLARATION,
	N_VECTOR_DECLARATION,
	N_DECLARATION,
	N_DECLARATION_SEQUENCE,
  N_TRANSLATION_UNIT
};

enum alternative {
	ALT_ATOMIC = 1,
	ALT_PRIMARY_EXPRESSION,
	ALT_NEGATION,
	ALT_MULTIPLICATION,
	ALT_MULTIPLICATIVE_EXPRESSION,
	ALT_ADDITION,
	ALT_ADDITIVE_EXPRESSION,
	ALT_ATOMIC_EXPRESSION,
	ALT_COMPONENTS,
	ALT_VECTOR,
	ALT_VECTOR_PROMARY_EXPRESSION,
	ALT_VECTOR_NEGATION,
	ALT_VECTOR_SCALAR_MULTIPLICATION,
	ALT_VECTOR_ADDITION,
	ALT_VECTOR_EXPRESSION,
	ALT_SCALAR_DECLARATION,
	ALT_VECTOR_DECLARATION,
	ALT_DECLARATION,
	ALT_DECLARATION_SEQUENCE,
	ALT_TRANSLATION_UNIT,

	ALT_IDENTIFIER,
	ALT_NUMBER,

	ALT_ADD,
	ALT_SUB,
	ALT_MULT,
	ALT_DIV
};

struct atomic {
	enum type type;
	enum alternative alternative;
	union {
		double number;
		const char *identifier;
	};
};

struct primary_expression {
	enum type type;
	enum alternative alternative;
	union {
		struct atomic_expression *atomic_expression;
		struct atomic *atomic;
	};
};

struct negation {
	enum type type;
	enum alternative alternative;
	union {
		struct negation *negation;
		struct primary_expression *primary_expression;
	};
};

struct multiplication {
	enum type type;
	enum alternative alternative;
	struct negation *negation0;
	struct negation *negation1;
};

struct multiplicative_expression {
	enum type type;
	enum alternative alternative;
	union {
		struct multiplication *multiplication;
		struct negation *negation;
	};
};

struct addition {
	enum type type;
	enum alternative alternative;
	struct multiplicative_expression *expression0;
	struct multiplicative_expression *expression1;
};

struct additive_expression {
	enum type type;
	enum alternative alternative;
	union {
		struct addition *addition;
		struct multiplicative_expression *multiplicative_expression;
	};
};

struct atomic_expression {
	enum type type;
	enum alternative alternative;
	struct additive_expression *additive_expression;
};

struct scalar_declaration {
	enum type type;
	enum alternative alternative;
	const char *identifier;
	struct atomic_expression *atomic_expression;
};

struct declaration {
	enum type type;
	enum alternative alternative;
	// struct vector_declaration *vector_declaration;
	struct scalar_declaration *scalar_declaration;
};

struct declaration_sequence {
	enum type type;
	enum alternative alternative;
	int size;
	struct declaration *declaration;
	struct declaration_sequence *declaration_sequence;
};

struct translation_unit {
	enum type type;
	enum alternative alternative;
	struct declaration_sequence *declaration_sequence;
};

#endif
