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
  int type;
  struct negation *negation0;
  struct negation *negation1;
};

struct multiplicative_expression {
  int type;
  union {
    struct multiplication *multiplication;
    struct negation *negation;
  };
};

struct addition {
  int type;
  struct multiplicative_expression *expression0;
  struct multiplicative_expression *expression1;
};

struct additive_expression {
  int type;
  union {
    struct addition *addition;
    struct multiplicative_expression *multiplicative_expression;
  };
};

struct atomic_expression {
  struct additive_expression *additive_expression;
};

struct scalar_declaration {
  struct identifier *identifier;
  struct atomic_expression *atomic_expression;
};

struct declaration {
  // struct vector_declaration *vector_declaration;
  struct scalar_declaration *scalar_declaration;
};

struct declaration_sequence {
  int size;
  struct declaration *declaration;
};

struct translation_unit {
  struct declaration_sequence *declaration_sequence;
};

#endif
