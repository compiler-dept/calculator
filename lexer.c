#include "lexer.h"

struct token *alloc_token(int value, int start, int end, const char *text){
    struct token *t = malloc(sizeof(struct token));
    t->value = value;
    t->text = malloc(strlen(text) + 1);
    strcpy(t->text, text);
    t->start = start;
    t->end = end;
    return t;
}

void free_token(struct token *t){
    free(t->text);
    free(t);
}

struct token *next_token(char *s, int offset){
    printf("Scan: ");
    puts(s+offset);
    const char *error;
    int erroroffset;
    pcre *re = pcre_compile("[a-zA-Z]+", 
            0, &error, &erroroffset, NULL);

    if (re == NULL){
        puts("Error compiling regex");
    }

    int rc;
    int ovector[30];
    rc = pcre_exec(re, NULL, s, strlen(s), offset, 0, ovector, 30); 
    pcre_free(re);

    if (rc > 0) {
        int len = ovector[1] - ovector[0];
        char match[len+1];
        match[len] = '\0';
        strncpy(match, s+ovector[0], len);
        puts(match);

        struct token *t = alloc_token(WORD, ovector[0], ovector[1], match);
        return t;
    }
    else {
        return NULL;
    }
}
