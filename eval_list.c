#include "eval_list.h"

#include <assert.h>
#include "eval_types.h"
#include "exception.h"
#include "refs.h"

static inline list_value_t *list_coerce(value_t *obj) {
    assert(obj->type == VAL_LIST);
    return (list_value_t *) obj;
}
static inline int64_t list_coerce_subscript(list_value_t *list, value_t *obj) {
    if (obj->type != VAL_INTEGER) {
        exception_set(EXC_TYPE_ERROR, "list indices must be integers");
        return 0;
    }

    int64_t idx = ((integer_value_t *) obj)->integer_value;
    /* If the subscript is negative, then add the size of the list to get
     * the actual index to use. */
    if (idx < 0) {
        idx += list->size;
    }

    /* If the subscript is still invalid, then raise an error. */
    if (idx < 0 || idx >= list->size) {
        exception_set(EXC_INDEX_ERROR, "list index out of bounds");
        return 0;
    }

    return idx;
}

static inline ref_array_value_t *list_refarray(list_value_t *list) {
    value_t *obj = deref(list->values);
    assert(obj->type == VAL_REF_ARRAY);
    return (ref_array_value_t *) obj;
}

bool list_bool(value_t *obj) {
    return list_len(obj) > 0;
}

int64_t list_len(value_t *obj) {
    return list_coerce(obj)->size;
}

int list_cmp(value_t *lobj, value_t *robj) {
    list_value_t *l = list_coerce(lobj);
    list_value_t *r = list_coerce(robj);
    reference_t *lvals = list_refarray(l)->values;
    reference_t *rvals = list_refarray(r)->values;
    int64_t lsize = l->size, rsize = r->size;
    int64_t min_size = lsize < rsize ? lsize : rsize;
    for (int64_t idx = 0; idx < min_size; idx++) {
        int comparison = compare(lvals[idx], rvals[idx]);
        if (exception_occurred()) {
            return 0;
        }
        if (comparison != 0) {
            return comparison;
        }
    }
    return (lsize > rsize) - (lsize < rsize);
}
bool list_eq(value_t *lobj, value_t *robj) {
    list_value_t *l = list_coerce(lobj);
    list_value_t *r = list_coerce(robj);

    if (l->size != r->size) {
        return false;
    }

    reference_t *lvals = list_refarray(l)->values;
    reference_t *rvals = list_refarray(r)->values;

    for (int64_t idx = 0; idx < l->size; idx++) {
        if (!ref_eq(lvals[idx], rvals[idx]) || exception_occurred()) {
            return false;
        }
    }

    return true;
}

/*! Implements subscript access for list types. */
reference_t list_subscr_get(value_t *obj, reference_t subscr) {
    /* First ensure that this is actually a list_value_t. */
    list_value_t *list = list_coerce(obj);

    /* Then check to make sure that the subscript is an integer. */
    int64_t idx = list_coerce_subscript(list, deref(subscr));
    if (exception_occurred()) {
        return NULL_REF;
    }

    /* Finally look up the appropriate reference in the list. */
    ref_array_value_t *array = list_refarray(list);
    incref(array->values[idx]);
    return array->values[idx];
}

/*! Implements subscript assignment for list types. */
void list_subscr_set(value_t *obj, reference_t subscr, reference_t value) {
    /* First ensure that this is actually a list_value_t. */
    list_value_t *list = list_coerce(obj);

    /* Then check to make sure that the subscript is an integer. */
    int64_t idx = list_coerce_subscript(list, deref(subscr));
    if (exception_occurred()) {
        return;
    }

    /* Finally set the appropriate reference in the list. */
    ref_array_value_t *array = list_refarray(list);
    decref(array->values[idx]);
    incref(value);
    array->values[idx] = value;
}

void list_subscr_del(value_t *obj, reference_t subscr) {
    /* First ensure that this is actually a list_value_t. */
    list_value_t *list = list_coerce(obj);

    /* Then check to make sure that the subscript is an integer. */
    int64_t idx = list_coerce_subscript(list, deref(subscr));
    if (exception_occurred()) {
        return;
    }

    /* Now get the value array so that we can actually perform the deletion
     * operation. */
    ref_array_value_t *array = list_refarray(list);

    /* Update the size of the list. */
    list->size--;

    /* Move any values after the element to be deleted up by one slot. */
    decref(array->values[idx]);
    for (int64_t i = idx; i < list->size; i++) {
        array->values[i] = array->values[i + 1];
    }
    array->values[list->size] = NULL_REF;
}

/*! Implements printing of lists. */
void list_print(value_t *obj, FILE *stream, size_t depth) {
    /* If we have reached the maximum recursion depth, print a placeholder. */
    if (depth == 0) {
        fprintf(stream, "...");
        return;
    }

    /* First ensure that this is actually a list_value_t. */
    list_value_t *list = list_coerce(obj);

    /* Then get the array of references. */
    ref_array_value_t *array = list_refarray(list);

    /* Then print out the contents. */
    fprintf(stream, "[");
    if (list->size >= 1) {
        ref_print_repr(array->values[0], stream, depth - 1);
        for (int64_t i = 1; i < list->size; i++) {
            fprintf(stream, ", ");
            ref_print_repr(array->values[i], stream, depth - 1);
        }
    }
    fprintf(stream, "]");
}
