#ifndef EVAL_DICT_H
#define EVAL_DICT_H

#include <stdbool.h>
#include <stdio.h>

#include "types.h"

// TYPE INTERFACE FUNCTIONS //

bool dict_bool(value_t *obj);
int64_t dict_len(value_t *obj);
bool dict_eq(value_t *l, value_t *r);
reference_t dict_subscr_get(value_t *obj, reference_t subscr);
void dict_subscr_set(value_t *obj, reference_t subscr, reference_t value);
void dict_subscr_del(value_t *obj, reference_t subscr);
void dict_print(value_t *obj, FILE *stream, size_t depth);

#endif /* EVAL_DICT_H */
