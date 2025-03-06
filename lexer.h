#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "list.h"

typedef struct lex{
    char *lexeme;
    token_t token;
    unsigned int pos;
} lex_t;

lex_t *lex_create(char *lexeme, token_t token, unsigned int pos);
void lex_destroy(lex_t *lex);

int lex_analysis(char *source, unsigned int len, list_t *lexes);

#endif
