#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stddef.h>

#include "arena.h"
#include "types.h"

typedef enum NodeType {
    STMT_SEQUENCE,

    STMT_ASSIGN,
    STMT_DEL,
    STMT_IF,
    STMT_WHILE,

    STMT_SENTRY_LAST,       /*!< This is here to make implementing
                             *   `is_expression` simpler. */

    EXPR_LITERAL_STRING,
    EXPR_LITERAL_INTEGER,
    EXPR_LITERAL_FLOAT,
    EXPR_LITERAL_LIST,
    EXPR_LITERAL_DICT,
    EXPR_LITERAL_SINGLETON,

    EXPR_IDENTIFIER,
    EXPR_NOT_TEST,
    EXPR_AND_TEST,
    EXPR_OR_TEST,
    EXPR_BUILTIN,
    EXPR_CALL,
    EXPR_SUBSCRIPT
} NodeType;

static inline bool is_statement(NodeType type) {
    return type <= STMT_SENTRY_LAST;
}

typedef struct Node {
    NodeType type;
} Node;

typedef struct NodeListEntry NodeListEntry;
struct NodeListEntry {
    NodeListEntry *next;
    Node *node;
};

typedef struct NodeList {
    size_t length;
    NodeListEntry *head;
    NodeListEntry *tail;
} NodeList;

typedef struct NodeStmtSequence {
    NodeType type;
    NodeList *statements;
} NodeStmtSequence;

typedef struct NodeStmtAssign {
    NodeType type;
    Node *left;
    Node *right;
} NodeStmtAssign;

typedef struct NodeStmtDel {
    NodeType type;
    Node *arg;
} NodeStmtDel;

typedef struct NodeStmtIf {
    NodeType type;
    Node *cond;
    Node *left;
    Node *right;
} NodeStmtIf;

typedef struct NodeStmtWhile {
    NodeType type;
    Node *cond;
    Node *body;
} NodeStmtWhile;

typedef struct NodeExprLiteralString {
    NodeType type;
    const char *value;
} NodeExprLiteralString;

typedef struct NodeExprLiteralInteger {
    NodeType type;
    int64_t value;
} NodeExprLiteralInteger;

typedef struct NodeExprLiteralList {
    NodeType type;
    NodeList *values;
} NodeExprLiteralList;

typedef struct NodeExprLiteralDict {
    NodeType type;
    NodeList *keys;
    NodeList *values;
} NodeExprLiteralDict;

typedef enum SingletonType {
    S_NONE,
    S_TRUE,
    S_FALSE
} SingletonType;

typedef struct NodeExprLiteralSingleton {
    NodeType type;
    SingletonType singleton;
} NodeExprLiteralSingleton;


typedef struct NodeExprIdentifier {
    NodeType type;
    const char *name;
} NodeExprIdentifier;

typedef struct NodeExprNotTest {
    NodeType type;
    Node *operand;
} NodeExprNotTest;

typedef struct NodeExprAndTest {
    NodeType type;
    Node *left;
    Node *right;
} NodeExprAndTest;

typedef struct NodeExprOrTest {
    NodeType type;
    Node *left;
    Node *right;
} NodeExprOrTest;

typedef enum NodeExprBuiltinType {
    UOP_NEGATE,
    UOP_IDENTITY,

    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MODULO,

    COMP_EQUALS,
    COMP_LT,
    COMP_GT,
    COMP_LE,
    COMP_GE
} NodeExprBuiltinType;

static inline bool is_unary_builtin(NodeExprBuiltinType type) {
    return type == UOP_NEGATE || type == UOP_IDENTITY;
}

typedef struct NodeExprBuiltin {
    NodeType type;
    NodeExprBuiltinType builtin_type;
    Node *left;
    Node *right;
} NodeExprBuiltin;

typedef struct NodeExprCall {
    NodeType type;
    Node *func;
    NodeList *args;
} NodeExprCall;

typedef struct NodeExprSubscript {
    NodeType type;
    Node *obj;
    Node *index;
} NodeExprSubscript;

typedef struct ast {
    arena_t *arena;
    Node *root;
} ast_t;

void ast_init(ast_t *ast);
void ast_destroy(ast_t *ast);

NodeList *ast_alloc_nodelist(ast_t *ast);

void ast_nodelist_append(ast_t *ast, NodeList *list, Node *node);
size_t ast_nodelist_length(NodeList *list);

Node *ast_alloc_sequence(ast_t *ast, NodeList *statements);
Node *ast_alloc_assign(ast_t *ast, Node *left, Node *right);
Node *ast_alloc_del(ast_t *ast, Node *arg);
Node *ast_alloc_if(ast_t *ast, Node *cond, Node *left, Node *right);
Node *ast_alloc_while(ast_t *ast, Node *cond, Node *body);

Node *ast_alloc_literal_string(ast_t *ast, const char *value);
Node *ast_alloc_literal_integer(ast_t *ast, int64_t value);
Node *ast_alloc_literal_list(ast_t *ast, NodeList *values);
Node *ast_alloc_literal_dict(ast_t *ast, NodeList *keys, NodeList *values);
Node *ast_alloc_literal_singleton(ast_t *ast, SingletonType type);

Node *ast_alloc_identifier(ast_t *ast, const char *name);
Node *ast_alloc_not_test(ast_t *ast, Node *operand);
Node *ast_alloc_and_test(ast_t *ast, Node *left, Node *right);
Node *ast_alloc_or_test(ast_t *ast, Node *left, Node *right);
Node *ast_alloc_builtin(ast_t *ast, NodeExprBuiltinType type, Node *left, Node *right);
Node *ast_alloc_call(ast_t *ast, Node *func, NodeList *args);
Node *ast_alloc_subscript(ast_t *ast, Node *obj, Node *index);

#endif /* AST_H */
