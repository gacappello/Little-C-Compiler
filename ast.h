#ifndef AST_H
#define AST_H

#include "token.h"

typedef enum asttype {
    AST_TRANSLATION_UNIT = 0,
    AST_BINARY_OPERATOR,
    AST_UNARY_OPERATOR,
    AST_LITERAL,
    AST_IDENTIFIER,
    AST_STMT,
    AST_CNT,
} asttype_t;

enum ast_literal_type {
    AST_LITERAL_INT = 0,
    AST_LITERAL_FLOAT,
    AST_LITERAL_STRING,
    AST_LITERAL_CHAR,
};

enum ast_stmt_type {
    AST_STMT_COMPOUND = 0,
    AST_STMT_EXPR,
    AST_STMT_SELECTION,
    AST_STMT_ITERATON,
    AST_STMT_JUMP,
};

enum ast_stmt_decl_type {
    AST_STMT_DECL_FUNC = 0,
    AST_STMT_DECL_DECL,
};

struct ast_translation_unit {
    struct ast *decls;
};

struct ast_binary_operator {
    struct ast *left;
    struct ast *right;
    token_t op;
};

struct ast_unary_operator {
    struct ast *operand;
    token_t op;
};

struct ast_literal {
    union {
        
    };
    enum ast_literal_type type;
};

struct ast_identifier {
    char *identifier;
};

struct ast_statement {
    union {
        struct { struct ast *next; } stmt_compound;
        struct { struct ast *left; struct ast *right; } stmt_expression;
        struct { struct ast *expr; struct ast *body; struct ast *next; } stmt_selection;
        struct { struct ast *init; struct ast *expr; struct ast *steps; struct ast *body; } stmt_iter;
        struct { struct ast *expr; } stmt_jump;
    };
    enum ast_stmt_type type;
};

typedef struct ast {
    union {
        struct ast_translation_unit translation_unit;
        struct ast_binary_operator binary_operator;
        struct ast_unary_operator unary_operator;
        struct ast_literal literal;
        struct ast_identifier identifier;
    };
    asttype_t type;
    struct ast *next;
} ast_t;

ast_t *ast_create(asttype_t type);

#endif
