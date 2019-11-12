#ifndef EVAL_TYPES_H
#define EVAL_TYPES_H

#include <stdio.h>

#include "ast.h"
#include "types.h"

/*! Max depth to print to. */
#define MAX_DEPTH 4

//// TYPE SPECIFIC FUNCTIONS

reference_t singleton_to_ref(SingletonType type);
reference_t bool_ref(bool value);

//// GENERIC REFERENCE FUNCTIONS ////

void eval_types_init(void);

const char *type_to_str(value_type_t type);

bool ref_is_none(reference_t r);
bool ref_is_true(reference_t r);
bool ref_is_false(reference_t r);

value_type_t ref_type(reference_t r);
const char *ref_typestr(reference_t r);

bool ref_bool(reference_t r);
int64_t ref_len(reference_t r);
uint64_t ref_hash(reference_t r);

reference_t ref_builtin(NodeExprBuiltinType type, reference_t l, reference_t r);
int compare(reference_t l, reference_t r);
reference_t ref_compare(NodeExprBuiltinType type, reference_t l, reference_t r);
bool ref_eq(reference_t l, reference_t r);

reference_t ref_subscr_get(reference_t r, reference_t subscr);
void ref_subscr_set(reference_t r, reference_t subscr, reference_t value);
void ref_subscr_del(reference_t r, reference_t subscr);

void ref_print(reference_t r, FILE *stream, size_t depth);
void ref_println(reference_t r, FILE *stream, size_t depth);

void ref_print_repr(reference_t r, FILE *stream, size_t depth);
void ref_println_repr(reference_t r, FILE *stream, size_t depth);

#endif /* EVAL_TYPES_H */
