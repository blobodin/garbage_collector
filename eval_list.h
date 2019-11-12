#ifndef EVAL_LIST_H
#define EVAL_LIST_H

#include <stdbool.h>
#include <stdio.h>

#include "types.h"

// TYPE INTERFACE FUNCTIONS //

bool list_bool(value_t *obj);
int64_t list_len(value_t *obj);
int list_cmp(value_t *lobj, value_t *robj);
bool list_eq(value_t *lobj, value_t *robj);
reference_t list_subscr_get(value_t *obj, reference_t subscr);
void list_subscr_set(value_t *obj, reference_t subscr, reference_t value);
void list_subscr_del(value_t *obj, reference_t subscr);
void list_print(value_t *obj, FILE *stream, size_t depth);

#endif /* EVAL_LIST_H */