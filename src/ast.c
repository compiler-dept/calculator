#include "ast.h"
#include "parser_state.h"
#include "../tests/test_helper.h"
#include <stdio.h>

char *AST_TYPE_NAMES[] = {
    "Atomic", "Primary_Expression", "Negation", "Multiplication",
    "Multiplicative_Expression", "Addition", "Additive_Expression",
    "Atomic_Expression", "Components", "Vector",
    "Vector_Primary_Expression", "Vector_Negation",
    "Vector_Scalar_Multiplication", "Vector_Addition",
    "Vector_Expression", "Scalar_Declaration", "Vector_Declaration",
    "Declaration", "Declaration_Sequence", "Translation_Unit"
};

void payload_free(void *payload)
{
    if (payload) {
        if (((struct payload *)(payload))->type == N_DECLARATION && ((struct payload *)(payload))->alternative == ALT_EXPRESSION) {
            free((char *)(((struct payload *)(payload))->declaration.identifier));
        }

        if (((struct payload *)(payload))->type == N_ATOMIC && ((struct payload *)(payload))->alternative == ALT_IDENTIFIER) {
            free((char *)(((struct payload *)(payload))->atomic.identifier));
        }

        free((struct payload *)payload);
    }
}
