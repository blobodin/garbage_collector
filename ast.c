#include "ast.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


void ast_init(ast_t *ast) {
    assert(ast != NULL);
    ast->arena = arena_new();
}
void ast_destroy(ast_t *ast) {
    assert(ast != NULL);
    arena_free(ast->arena);
}

NodeList *ast_alloc_nodelist(ast_t *ast) {
    NodeList *list = arena_malloc(ast->arena, sizeof(*list));
    if (list) {
        memset(list, 0, sizeof(*list));
    }
    return list;
}

static NodeListEntry *ast_alloc_nodelistentry(ast_t *ast, Node *node) {
    NodeListEntry *entry = arena_malloc(ast->arena, sizeof(*entry));
    if (entry) {
        entry->next = NULL;
        entry->node = node;
    }
    return entry;
}

void ast_nodelist_append(ast_t *ast, NodeList *list, Node *node) {
    assert(list != NULL);

    NodeListEntry *entry = ast_alloc_nodelistentry(ast, node);
    if (list->tail) {
        list->tail->next = entry;
        list->tail = entry;
    } else {
        list->head = list->tail = entry;
    }
    list->length++;
}

size_t ast_nodelist_length(NodeList *list) {
    assert(list != NULL);

    return list->length;
}

#define AST_NODE_DECL(TYPENAME, TYPE) \
    TYPENAME *node = (TYPENAME *) ast_alloc_node(ast, sizeof(TYPENAME), TYPE)

static Node *ast_alloc_node(ast_t *ast, size_t size, NodeType type) {
    Node *node = arena_malloc(ast->arena, size);
    if (node) {
        node->type = type;
    }
    return node;
}

Node *ast_alloc_sequence(ast_t *ast, NodeList *statements) {
    AST_NODE_DECL(NodeStmtSequence, STMT_SEQUENCE);
    if (node) {
        node->statements = statements;
    }
    return (Node *) node;
}

Node *ast_alloc_assign(ast_t *ast, Node *left, Node *right) {
    AST_NODE_DECL(NodeStmtAssign, STMT_ASSIGN);
    if (node) {
        node->left = left;
        node->right = right;
    }
    return (Node *) node;
}
Node *ast_alloc_del(ast_t *ast, Node *arg) {
    AST_NODE_DECL(NodeStmtDel, STMT_DEL);
    if (node) {
        node->arg = arg;
    }
    return (Node *) node;
}
Node *ast_alloc_if(ast_t *ast, Node *cond, Node *left, Node *right) {
    AST_NODE_DECL(NodeStmtIf, STMT_IF);
    if (node) {
        node->cond = cond;
        node->left = left;
        node->right = right;
    }
    return (Node *) node;
}
Node *ast_alloc_while(ast_t *ast, Node *cond, Node *body) {
    AST_NODE_DECL(NodeStmtWhile, STMT_WHILE);
    if (node) {
        node->cond = cond;
        node->body = body;
    }
    return (Node *) node;
}

Node *ast_alloc_literal_string(ast_t *ast, const char *value) {
    AST_NODE_DECL(NodeExprLiteralString, EXPR_LITERAL_STRING);
    if (node) {
        node->value = arena_strdup(ast->arena, value);
    }
    return (Node *) node;
}
Node *ast_alloc_literal_integer(ast_t *ast, int64_t value) {
    AST_NODE_DECL(NodeExprLiteralInteger, EXPR_LITERAL_INTEGER);
    if (node) {
        node->value = value;
    }
    return (Node *) node;
}
Node *ast_alloc_literal_list(ast_t *ast, NodeList *values) {
    AST_NODE_DECL(NodeExprLiteralList, EXPR_LITERAL_LIST);
    if (node) {
        node->values = values;
    }
    return (Node *) node;
}
Node *ast_alloc_literal_dict(ast_t *ast, NodeList *keys, NodeList *values) {
    AST_NODE_DECL(NodeExprLiteralDict, EXPR_LITERAL_DICT);
    if (node) {
        node->keys = keys;
        node->values = values;
    }
    return (Node *) node;
}
Node *ast_alloc_literal_singleton(ast_t *ast, SingletonType type) {
    AST_NODE_DECL(NodeExprLiteralSingleton, EXPR_LITERAL_SINGLETON);
    if (node) {
        node->singleton = type;
    }
    return (Node *) node;
}

Node *ast_alloc_identifier(ast_t *ast, const char *name) {
    AST_NODE_DECL(NodeExprIdentifier, EXPR_IDENTIFIER);
    if (node) {
        node->name = arena_strdup(ast->arena, name);
    }
    return (Node *) node;
}

Node *ast_alloc_not_test(ast_t *ast, Node *operand) {
    AST_NODE_DECL(NodeExprNotTest, EXPR_NOT_TEST);
    if (node) {
        node->operand = operand;
    }
    return (Node *) node;
}
Node *ast_alloc_and_test(ast_t *ast, Node *left, Node *right) {
    AST_NODE_DECL(NodeExprAndTest, EXPR_AND_TEST);
    if (node) {
        node->left = left;
        node->right = right;
    }
    return (Node *) node;
}
Node *ast_alloc_or_test(ast_t *ast, Node *left, Node *right) {
    AST_NODE_DECL(NodeExprOrTest, EXPR_OR_TEST);
    if (node) {
        node->left = left;
        node->right = right;
    }
    return (Node *) node;
}

Node *ast_alloc_builtin(ast_t *ast, NodeExprBuiltinType type, Node *left, Node *right) {
    AST_NODE_DECL(NodeExprBuiltin, EXPR_BUILTIN);
    if (node) {
        node->builtin_type = type;
        node->left = left;
        node->right = right;
    }
    return (Node *) node;
}

Node *ast_alloc_call(ast_t *ast, Node *func, NodeList *args) {
    AST_NODE_DECL(NodeExprCall, EXPR_CALL);
    if (node) {
        node->func = func;
        node->args = args;
    }
    return (Node *) node;
}

Node *ast_alloc_subscript(ast_t *ast, Node *obj, Node *index) {
    AST_NODE_DECL(NodeExprSubscript, EXPR_SUBSCRIPT);
    if (node) {
        node->obj = obj;
        node->index = index;
    }
    return (Node *) node;
}
