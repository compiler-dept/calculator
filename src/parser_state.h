#ifndef PARSER_STATE_H
#define PARSER_STATE_H

#include "ast.h"

enum states {
	ERROR = 1,
	OK
};

struct parser_state {
	enum states state;
	//struct translation_unit *translation_unit;
	struct multiplication *multiplication;
};

#endif
