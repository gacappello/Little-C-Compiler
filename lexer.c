#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "lexer.h"
#include "token.h"
#include "list.h"

static char *buf;
static char *end;
static char *pos;

static inline char get()
{
    if(pos < end) return *pos;
    return '\0';
}

static inline char advance()
{
    if(pos < end) return *pos++;
    return '\0';
}

static inline int at_end()
{
    return pos == end;
}

static inline int match(char c)
{
    if(at_end()) return 0;
    if(c != *pos) return 0;
    pos++;
    return 1;
}

static inline void remove_blanks()
{
    char c;
    while((c = get()) == ' ' || c == '\t' 
            || c == '\n' || c == '\r')
        advance();
}

static int isoctal(char c)
{
    return c >= 48 && c <= 55;    
}

static void int_suffixes()
{
    (void)
    ((
     (match('U') || match('u')) && 
     (match('L') || match('l')) && 
     (match('L') || match('l'))
    ) 
        ||
    (
     (match('L') || match('l')) && 
     (match('L') || match('l'))
    ));
}

static void float_suffixes()
{
    (void)
    (
     (match('F') || match('f'))
        ||
     (match('L') || match('l'))
    );
}

lex_t *lex_create(char *lexeme, token_t token, unsigned int pos)
{
    lex_t *lex = calloc(1, sizeof(lex_t));
    lex->lexeme = lexeme;
    lex->token = token;
    lex->pos = pos;
    return lex;
}

void lex_destroy(lex_t *lex)
{
    if(!lex) return;
    free(lex->lexeme);
    free(lex);
}

static int is_keyword(char *s, int len, token_t *type)
{
    int rc = 0;
    char *str = strndup(s, len);
    if(strcmp(str, "auto" ) == 0) 
		{ *type = TOK_AUTO; rc = 1; }
    else if(strcmp(str, "break") == 0) 
		{ *type = TOK_BREAK; rc = 1; }
    else if(strcmp(str, "case") == 0) 
		{ *type = TOK_CASE; rc = 1; }
    else if(strcmp(str, "char") == 0) 
		{ *type = TOK_CHAR; rc = 1; }
    else if(strcmp(str, "const") == 0) 
		{ *type = TOK_CONST; rc = 1; }
    else if(strcmp(str, "continue") == 0) 
		{ *type = TOK_CONTINUE; rc = 1; }
    else if(strcmp(str, "default") == 0) 
		{ *type = TOK_DEFAULT; rc = 1; }
    else if(strcmp(str, "do") == 0) 
		{ *type = TOK_DO; rc = 1; }
    else if(strcmp(str, "double") == 0) 
		{ *type = TOK_DOUBLE; rc = 1; }
    else if(strcmp(str, "else") == 0) 
		{ *type = TOK_ELSE; rc = 1; }
    else if(strcmp(str, "enum") == 0) 
		{ *type = TOK_ENUM; rc = 1; }
    else if(strcmp(str, "extern") == 0) 
		{ *type = TOK_EXTERN; rc = 1; }
    else if(strcmp(str, "float") == 0) 
		{ *type = TOK_FLOAT; rc = 1; }
    else if(strcmp(str, "for") == 0) 
		{ *type = TOK_FOR; rc = 1; }
    else if(strcmp(str, "goto") == 0) 
		{ *type = TOK_GOTO; rc = 1; }
    else if(strcmp(str, "if") == 0) 
		{ *type = TOK_IF; rc = 1; }
    else if(strcmp(str, "int") == 0) 
		{ *type = TOK_INT; rc = 1; }
    else if(strcmp(str, "long") == 0) 
		{ *type = TOK_LONG; rc = 1; }
    else if(strcmp(str, "register") == 0) 
		{ *type = TOK_REGISTER; rc = 1; }
    else if(strcmp(str, "return") == 0) 
		{ *type = TOK_RETURN; rc = 1; }
    else if(strcmp(str, "short") == 0) 
		{ *type = TOK_SHORT; rc = 1; }
    else if(strcmp(str, "signed") == 0) 
		{ *type = TOK_SIGNED; rc = 1; }
    else if(strcmp(str, "sizeof") == 0) 
		{ *type = TOK_SIZEOF; rc = 1; }
    else if(strcmp(str, "static") == 0) 
		{ *type = TOK_STATIC; rc = 1; }
    else if(strcmp(str, "struct") == 0) 
		{ *type = TOK_STRUCT; rc = 1; }
    else if(strcmp(str, "switch") == 0) 
		{ *type = TOK_SWITCH; rc = 1; }
    else if(strcmp(str, "typedef") == 0) 
		{ *type = TOK_TYPEDEF; rc = 1; }
    else if(strcmp(str, "union") == 0) 
		{ *type = TOK_UNION; rc = 1; }
    else if(strcmp(str, "unsigned") == 0) 
		{ *type = TOK_UNSIGNED; rc = 1; }
    else if(strcmp(str, "void") == 0) 
		{ *type = TOK_VOID; rc = 1; }
    else if(strcmp(str, "volatile") == 0) 
		{ *type = TOK_VOLATILE; rc = 1; }
    else if(strcmp(str, "while") == 0) 
		{ *type = TOK_WHILE; rc = 1; }
    free(str);
    return rc;
}

static lex_t *get_next_lex()
{
    remove_blanks();

    char *lexeme = NULL;
    token_t type = TOK_INVALID;

    char c = advance();
    switch(c) {
    case '(':
        type = TOK_OPAREN;
        break;
    case ')':
        type = TOK_CPAREN;
        break;
    case '[':
        type = TOK_OBRACK;
        break;
    case ']':
        type = TOK_CBRACK;
        break;
    case '{':
        type = TOK_OBRACE;
        break;
    case '}':
        type = TOK_CBRACE;
        break;
    case ';':
        type = TOK_SEMICOLON;
        break;
    case ':':
        type = TOK_COLON;
        break;
    case '+':
        type = TOK_PLUS;
        if(match('+')) type = TOK_INC;
        else if(match('=')) type = TOK_PLUS_EQ;
        break;
    case '-':
        type = TOK_MINUS;
        if(match('-')) type = TOK_DEC;
        else if(match('=')) type = TOK_MINUS_EQ;
        else if(match('>')) type = TOK_ARROW;
        break;
    case '*':
        type = TOK_STAR;
        if(match('=')) type = TOK_STAR_EQ;
        break;
    case '/':
        type = TOK_SLASH;
        if(match('=')) type = TOK_SLASH_EQ;
        break;
    case '%':
        type = TOK_MODULO;
        if(match('=')) type = TOK_MODULO_EQ;
        break;
    case '=':
        type = TOK_EQ;
        if(match('=')) type = TOK_EQ_TO;
        break;
    case '!':
        type = TOK_BANG;
        if(match('=')) type = TOK_NEQ_TO;
        break;
    case '>':
        type = TOK_GT;
        if(match('=')) type = TOK_GTE;
        else if(match('>')) {
            type = TOK_SR;
            if(match('=')) type = TOK_SR_EQ;
        }
        break;
    case '<':
        type = TOK_LT;
        if(match('=')) type = TOK_LTE;
        else if(match('<')) {
            type = TOK_SL;
            if(match('=')) type = TOK_SL_EQ;
        }
        break;
    case '&':
        type = TOK_AND;
        if(match('=')) type = TOK_AND_EQ;
        else if(match('&')) type = TOK_AND_AND;
        break;
    case '|':
        type = TOK_OR;
        if(match('=')) type = TOK_OR_EQ;
        else if(match('|')) type = TOK_OR_OR;
        break;
    case '~':
        type = TOK_TILDE;
        break;
    case '^':
        type = TOK_XOR;
        if(match('=')) type = TOK_XOR_EQ;
        break;
    case '.':
        type = TOK_DOT;
        break;
    case ',':
        type = TOK_COMMA;
        break;
    case '?':
        type = TOK_QUESTION;
        break;
    case '\0':
        type = TOK_EOF;
        break;
    }

    if(c == '/' && get() == '/') {
        char *p = pos - 1;
        char n = get();
        int size;
        type = TOK_COMMENT;
        advance();
        while(n != '\n') {
            advance();
            n = get();
        }
        size = (int)(pos - p);
        lexeme = strndup(p, size);
    }

    if(isalpha(c)) {
        char n = get();
        char *p = pos - 1;
        int size;
        type = TOK_IDENTIFIER;
        while(isalnum(n) || n == '_') {
            advance();
            n = get();
        }
        size = (int)(pos - p);
        if(!is_keyword(p, size, &type))
            lexeme = strndup(p, size);
    }

    if(isdigit(c) || c == '.') {
        char n = get();
        char *p = pos - 1;
        int size;
        type = TOK_INT_LITERAL;
        // hex
        if(c == '0' && (n == 'X' || n == 'x')) {
            advance();
            n = get();
            while(isxdigit(n)) {
                advance();
                n = get();
            }
        }
        // octal
        else if (c == '0') {
            while(isoctal(n)) {
                advance();
                n = get();
            }
        }
        // dec or float
        else {
            int dot = 0;
            if(c == '.') dot = 1;
            while(isdigit(n)) {
                advance();
                n = get();
            }
            if(n == '.' && !dot) {
                dot = 1;
                advance();
                n = get();
                while(isdigit(n)) {
                    advance();
                    n = get();
                }
            }
            if(n == 'E' || n == 'e') {
                advance();
                n = get();
                if(n == '-') {
                    advance();
                    n = get();
                }
                while(isdigit(n)) {
                    advance();
                    n = get();
                }
            }
            if(dot) type = TOK_FLOAT_LITERAL;
        }

        if(type == TOK_INT_LITERAL)
            int_suffixes();
        else if(type == TOK_FLOAT_LITERAL)
            float_suffixes();

        size = (int)(pos - p);
        lexeme = strndup(p, size);
    }

    if(c == '\'') {
        char n = get();
        char *p = pos - 1;
        int size;
        type = TOK_CHAR_LITERAL;
        if(n == '\\')
            advance();
        n = advance();
        if(n == '\'') type = TOK_INVALID;
        n = get();
        if(n == '\'')
            advance();
        else type = TOK_INVALID;
        size = (int)(pos - p);
        lexeme = strndup(p, size);
    }

    if(c == '"') {
        char *p = pos - 1;
        char n;
        int size;
        while((n = advance()) != '"'){}
        size = (int)(pos - p);
        lexeme = strndup(p, size);
        type = TOK_STRING_LITERAL;
    }

    return lex_create(lexeme, type, pos - buf);
}

int lex_analysis(char *source, unsigned int len, list_t *lexes)
{
    assert(lexes);

    buf = source;
    end = buf + len;
    pos = buf;

    int rc = 1;
    while(pos < end) {
        lex_t *l = get_next_lex();
        if(l->token == TOK_INVALID) rc = 0;
        list_push(lexes, l, (void (*)(void*))lex_destroy);
    }

    return rc;
}
