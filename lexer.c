#include "lexer.h"

struct token *alloc_token(int value, char* text){
    struct token *t = malloc(sizeof(struct token));
    t->value = value;
    t->text = malloc(strlen(text));
    strcpy(t->text, text);
    return t;
}

void free_token(struct token *t){
    free(t->text);
    free(t);
}
