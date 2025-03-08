#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "lexer.h"
#include "token.h"

int main(int argc, char **argv)
{
    if(argc != 2) {
        fprintf(stderr, "%s <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *f = fopen(argv[1], "r");
    if(!f) {
        perror("lc");
        return EXIT_FAILURE;
    }

    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    rewind(f);

    char *source = malloc(len);
    fread(source, 1, len, f);

    list_t *lexes = list_create();
    int res = lex_analysis(source, len, lexes);
    if(!res) {
        printf("Lexer found error\n");
    }

    list_node_t *n = lexes->head;
    for(int i=0; n; n = n->next, i++) {
        lex_t *l = (lex_t*)n->data;
        printf("%3d: tok: %2d:%-20s \t lexeme: %s\n", 
            i, l->token, tok_to_str(l->token), l->lexeme);
    }

    list_destroy(lexes);

    return 0;
}
