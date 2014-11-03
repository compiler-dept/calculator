#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "hashmap.h"
#include "ast_eval.h"
#include "gram.h"
#include "parser_state.h"

void *ParseAlloc(void *(*allocProc) (size_t));
void Parse(void *, int, const char *, struct parser_state *);
void ParseFree(void *, void (*freeProc) (void *));

int main()
{
    char *s = NULL;
    size_t len = 0;
    int r = 0;
    int token;
    struct parser_state parser_state;
    struct hashmap *mappings = NULL;
    yyscan_t scanner;
    yylex_init(&scanner);

    while (1) {
        printf("Input: ");
        getline(&s, &len, stdin);
        if (strncmp(s, "exit", 4) == 0) {
            free(s);
            break;
        }
        YY_BUFFER_STATE bufferState = yy_scan_string(s, scanner);

        int token = 0;
        void *shellParser = ParseAlloc(malloc);
        while ((token = yylex(scanner))) {
            Parse(shellParser, token, strdup(yyget_text(scanner)),
                  &parser_state);
        }
        free(s);
        s = NULL;

        Parse(shellParser, 0, NULL, &parser_state);

        ast_eval(parser_state.root, &mappings);
        ast_free(parser_state.root);
        for (int i = 0; i < mappings->capacity; i++) {
            if (mappings->values[i].key) {
                printf("%s: %f\n", mappings->values[i].key,
                       *((double *)mappings->values[i].value));
            }
        }
        yy_delete_buffer(bufferState, scanner);
        ParseFree(shellParser, free);
    }

    if (mappings) {
        for (int i = 0; i < mappings->capacity; i++) {
            if (mappings->values[i].key) {
                free(mappings->values[i].value);
            }
        }
    }

    hashmap_free(mappings);
    yylex_destroy(scanner);

    return 0;
}
