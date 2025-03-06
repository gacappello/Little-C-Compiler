#ifndef AST_H
#define AST_H

#include "list.h"

typedef enum asttype {
    AST_TRANSLATION_UNIT = 0,
    AST_EXTERNAL_DECLARATION,
    AST_FUNCTION_DEFINITION,
    AST_DECLARATION_SPECIFIER,

    AST_STORAGE_CLASS_SPECIFIER,

    AST_TYPE_SPECIFIER,
    AST_STRUCT_OR_UNION_SPECIFIER,
    AST_STRUCT_OR_UNION,
    AST_STRUCT_DECLARATION,
    AST_SPECIFIER_QUALIFIER,
    AST_STRUCT_DECLARATOR_LIST,
    AST_STRUCT_DECLARATOR,
    AST_DECLARATOR,
    AST_POINTER,
    AST_TYPE_QUALIFIER,
    AST_DIRECT_DECLARATOR,
    AST_CONSTANT_EXPRESSION,
    AST_CONDITIONAL_EXPRESSION,
    AST_LOGICAL_OR_EXPRESSION,
    AST_LOGICAL_AND_EXPRESSION,
    AST_INCLUSIVE_OR_EXPRESSION,
    AST_EXCLUSIVE_OR_EXPRESSION,
    AST_AND_EXPRESSION,
    AST_EQUALITY_EXPRESSION,
    AST_RELATIONAL_EXPRESSION,
    AST_SHIFT_EXPRESSION,
    AST_ADDITIVE_EXPRESSION,
    AST_MULTIPLICATIVE_EXPRESSION,
    AST_CAST_EXPRESSION,
    AST_UNARY_EXPRESSION,
    AST_POSTFIX_EXPRESSION,
    AST_PRIMARY_EXPRESSION,
    AST_CONSTANT,
    AST_EXPRESSION,
    AST_ASSIGNMENT_EXPRESSION,
    AST_ASSIGNMENT_OPERATOR,
    AST_UNARY_OPERATOR,
    AST_TYPE_NAME,
    AST_PARAMETER_TYPE_LIST,
    AST_PARAMETER_LIST,
    AST_PARAMETER_DECLARATION,
    AST_ABSTRACT_DECLARATOR,
    AST_DIRECT_ABSTRACT_DECLARATOR,
    AST_ENUM_SPECIFIER,
    AST_ENUMERATOR_LIST,
    AST_ENUMERATOR,
    AST_TYPEDEF_NAME,
    AST_DECLARATION,
    AST_INIT_DECLARATOR,
    AST_INITIALIZER,
    AST_INITIALIZER_LIST,
    AST_COMPOUND_STATEMENT,
    AST_STATEMENT,
    AST_LABELED_STATEMENT,
    AST_EXPRESSION_STATEMENT,
    AST_SELECTION_STATEMENT,
    AST_ITERATION_STATEMENT,
    AST_JUMP_STATEMENT,
    AST_CNT,
} asttype_t;

enum storage_class_specifier_type {
    AST_STORAGE_CLASS_SPECIFIER_NONE = 0
    AST_STORAGE_CLASS_SPECIFIER_AUTO,
    AST_STORAGE_CLASS_SPECIFIER_REGISTER,
    AST_STORAGE_CLASS_SPECIFIER_STATIC,
    AST_STORAGE_CLASS_SPECIFIER_EXTERN,
    AST_STORAGE_CLASS_SPECIFIER_TYPEDEF,
    AST_STORAGE_CLASS_SPECIFIER_COUNT,
};

enum type_specifier_type {
    AST_TYPE_SPECIFIER_VOID = 0,
    AST_TYPE_SPECIFIER_CHAR,
    AST_TYPE_SPECIFIER_SHORT,
    AST_TYPE_SPECIFIER_INT,
    AST_TYPE_SPECIFIER_LONG,
    AST_TYPE_SPECIFIER_FLOAT,
    AST_TYPE_SPECIFIER_DOUBLE,
    AST_TYPE_SPECIFIER_SIGNED,
    AST_TYPE_SPECIFIER_UNSIGNED,
    AST_TYPE_SPECIFIER_STRUCT_OR_UNION,
    AST_TYPE_SPECIFIER_ENUM_SPECIFIER,
    AST_TYPE_SPECIFIER_TYPEDEF_NAME,
    AST_TYPE_SPECIFIER_TYPEDEF_COUNT,
} 

enum type_qualifier_type {
    AST_TYPE_QUALIFIER_NONE = 0,
    AST_TYPE_QUALIFIER_CONST,
    AST_TYPE_QUALIFIER_VOLATILE,
    AST_TYPE_QUALIFIER_COUNT,
}

struct ast_translation_unit {
    list_t *ext_decls;
};

struct ast_external_declaration {
    union {
        struct ast *func_def;
        struct ast *decl;
    };
};

struct ast_function_definition {
    enum storage_class_specifier_type storage;
    union {
        struct ast *struct_or_union_spec;
        struct ast *enum_spec;
        struct ast *typedef_name;
    };
    enum type_specifier_type type_spec;
    enum type_qualifier_type type_quali;
    struct ast *declarator;
    list *declarations;
    struct ast *compound_statement;
};

typedef struct ast {
    union {
        struct ast_translation_unit translation_unit;
        struct ast_external_declaration external_declaration;
        struct ast_function_definition function_definition;
        struct ast_declaration declaration;
    };
    asttype_t type;
} ast_t;

#endif
