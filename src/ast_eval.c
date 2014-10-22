#include "ast_eval.h"

double evaluate_atomic(struct atomic atomic, struct hashmap *mappings)
{
    if (atomic.alternative == ALT_NUMBER){
        return atomic.number;
    } else {
        return hashmap_get(mappings);
    }
    
}
