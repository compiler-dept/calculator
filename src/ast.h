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
  N_TRANSLATION_UNIT,

};

struct atomic {
  enum type type;
  enum {
		AST_NUMBER = 1,
		AST_IDENTIFIER
	} alternative;
  union {
		double number;
		const char *identifier;
  };
};

struct primary_expression {
  enum type type;
  enum {
    AST_ATOMIC_EXPRESSION = 1,
    AST_ATOMIC
  } alternative;
  union {
    struct atomic_expression *atomic_expression;
    struct atomic *atomic;
  };
};

struct negation {
  enum type type;
  enum {
    AST_NEGATION = 1,
    AST_PRIMARY_EXPRESSION
  } alternative;
  union {
    struct negation *negation;
    struct primary_expression *primary_expression;
  };
};

struct multiplication {
  enum type type;
  enum {
    AST_MULTIPLICATION = 1,
    AST_DIVISION
  } alternative;
  struct negation *negation0;
  struct negation *negation1;
};

struct multiplicative_expression {
  enum type type;
  enum {
    AST_MULTIPLICATION = 1,
    AST_NEGATION
  } alternative;
  union {
    struct multiplication *multiplication;
    struct negation *negation;
  };
};

struct addition {
  enum type type;
  enum {
    AST_ADD = 1,
    AST_SUB
  } alternative;
  struct multiplicative_expression *expression0;
  struct multiplicative_expression *expression1;
};

struct additive_expression {
  enum type type;
  enum {
    AST_ADDITION = 1,
    AST_MULTIPLICATIVE_EXPRESSION
  } alternative;
  union {
    struct addition *addition;
    struct multiplicative_expression *multiplicative_expression;
  };
};

struct atomic_expression {
  enum type type;
  enum {
    AST_ADDITIVE_EXPRESSION = 1
  } alternative;
  struct additive_expression *additive_expression;
};

struct scalar_declaration {
  enum type type;
  enum {
    AST_ATOMIC_EXPRESSION = 1
  } alternative;
  const char *identifier;
  struct atomic_expression *atomic_expression;
};

struct declaration {
  enum type type;
  enum {
    AST_VECTOR_DECLARATION = 1,
    AST_SCALAR_DECLARATION
  } alternative;
  // struct vector_declaration *vector_declaration;
  struct scalar_declaration *scalar_declaration;
};

struct declaration_sequence {
  enum type type;
  enum {
    AST_DECLARATION = 1,
    AST_DECLARATION_SEQUENCE
  } alternative;
  int size;
  struct declaration *declaration;
  struct declaration_sequence *declaration_sequence;
};

struct translation_unit {
  enum type type;
  enum {
    AST_DECLARATION_SEQUENCE = 1
  } alternative;
  struct declaration_sequence *declaration_sequence;
};

#endif
