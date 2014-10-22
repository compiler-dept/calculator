#ifndef AST_H
#define AST_H

struct atomic {
  enum {
		AST_NUMBER = 1,
		AST_IDENTIFIER
	} type;
  union {
		double number;
		const char *identifier;
  };
};

struct primary_expression {
  enum {
    AST_ATOMIC_EXPRESSION = 1,
    AST_ATOMIC
  } type;
  union {
    struct atomic_expression *atomic_expression;
    struct atomic *atomic;
  };
};

struct negation {
  enum {
    AST_NEGATION = 1,
    AST_PRIMARY_EXPRESSION
  } type;
  union {
    struct negation *negation;
    struct primary_expression *primary_expression;
  };
};

struct multiplication {
  enum {
    AST_MULTIPLICATION = 1,
    AST_DIVISION
  } type;
  struct negation *negation0;
  struct negation *negation1;
};

struct multiplicative_expression {
  enum {
    AST_MULTIPLICATION = 1,
    AST_NEGATION
  } type;
  union {
    struct multiplication *multiplication;
    struct negation *negation;
  };
};

struct addition {
  enum {
    AST_ADD = 1,
    AST_SUB
  } type;
  struct multiplicative_expression *expression0;
  struct multiplicative_expression *expression1;
};

struct additive_expression {
  enum {
    AST_ADDITION = 1,
    AST_MULTIPLICATIVE_EXPRESSION
  } type;
  union {
    struct addition *addition;
    struct multiplicative_expression *multiplicative_expression;
  };
};

struct atomic_expression {
  enum {
    AST_ADDITIVE_EXPRESSION = 1
  } type;
  struct additive_expression *additive_expression;
};

struct scalar_declaration {
  enum {
    AST_ATOMIC_EXPRESSION = 1
  } type;
  const char *identifier;
  struct atomic_expression *atomic_expression;
};

struct declaration {
  enum {
    AST_VECTOR_DECLARATION = 1,
    AST_SCALAR_DECLARATION
  } type;
  // struct vector_declaration *vector_declaration;
  struct scalar_declaration *scalar_declaration;
};

struct declaration_sequence {
  enum {
    AST_DECLARATION = 1,
    AST_DECLARATION_SEQUENCE
  } type;
  int size;
  struct declaration *declaration;
  struct declaration_sequence *declaration_sequence;
};

struct translation_unit {
  enum {
    AST_DECLARATION_SEQUENCE = 1
  } type;
  struct declaration_sequence *declaration_sequence;
};

#endif
