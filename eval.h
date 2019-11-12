#ifndef EVAL_H
#define EVAL_H

#include <stdbool.h>

#include "ast.h"
#include "grammar.h"
#include "types.h"

void eval_init(void);
reference_t eval_root(Node *root);

bool ref_is_none(reference_t r);
bool ref_is_true(reference_t r);
bool ref_is_false(reference_t r);

size_t foreach_global(void (*f)(const char *name, reference_t ref));
void print_globals(void);

#endif /* EVAL_H */
