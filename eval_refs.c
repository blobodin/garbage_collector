#include "eval_refs.h"
#include "eval_types.h"

#include <string.h>

#include "refs.h"

//// GLOBAL VARIABLE DECLARATIONS ////

reference_t NONE_REF;
reference_t TRUE_REF;
reference_t FALSE_REF;

//// NEW REFERENCE FUNCTIONS ////

/*! Creates a new None reference. This should only be called once. */
reference_t make_reference_none(void) {
    return make_ref(VAL_NONE, sizeof(value_t));
}

/*! Creates a new Bool reference. This should only be called twice. */
reference_t make_reference_bool(void) {
    return make_ref(VAL_BOOL, sizeof(value_t));
}

/*! Assigns a long int to a new reference in the ref_table. */
reference_t make_reference_int(int64_t i) {
    reference_t ref = make_ref(VAL_INTEGER, sizeof(integer_value_t));
    if (ref != NULL_REF) {
        ((integer_value_t *) deref(ref))->integer_value = i;
    }
    return ref;
}


/*! Makes a reference for a new string of the given length. */
static reference_t make_reference_string_length(size_t len) {
    return make_ref(VAL_STRING, sizeof(string_value_t) + sizeof(char[len + 1]));
}

/*! Assigns a string to a new reference in the ref_table. */
reference_t make_reference_string(const char *value) {
    reference_t ref = make_reference_string_length(strlen(value));
    if (ref != NULL_REF) {
        strcpy(((string_value_t *) deref(ref))->string_value, value);
    }
    return ref;
}

/*! Assigns a concatenated string to a new reference in the ref_table. */
reference_t make_reference_string_concat(const char *v1, const char *v2) {
    size_t len1 = strlen(v1), len2 = strlen(v2);
    reference_t ref = make_reference_string_length(len1 + len2);
    if (ref != NULL_REF) {
        char *string_value = ((string_value_t *) deref(ref))->string_value;
        strcpy(string_value, v1);
        strcpy(string_value + len1, v2);
    }
    return ref;
}

/*! List allocation helper. */
reference_t make_reference_list() {
    reference_t ref = make_ref(VAL_LIST, sizeof(list_value_t));
    if (ref != NULL_REF) {
        list_value_t *lv = (list_value_t *) deref(ref);
        lv->size = 0;
        lv->values = NULL_REF;
    }
    return ref;
}

/*! Dict allocation helper. */
reference_t make_reference_dict() {
    reference_t ref = make_ref(VAL_DICT, sizeof(dict_value_t));
    if (ref != NULL_REF) {
        dict_value_t *dv = (dict_value_t *) deref(ref);
        dv->size = 0;
        dv->occupied = 0;
        dv->keys = NULL_REF;
        dv->values = NULL_REF;
    }
    return ref;
}

/*! RefArray allocation helper. */
reference_t make_reference_refarray(size_t capacity) {
    reference_t ref = make_ref(
        VAL_REF_ARRAY,
        sizeof(ref_array_value_t) + sizeof(reference_t[capacity])
    );
    if (ref != NULL_REF) {
        ref_array_value_t *rav = (ref_array_value_t *) deref(ref);
        rav->capacity = capacity;
        for (size_t i = 0; i < capacity; i++) {
            rav->values[i] = NULL_REF;
        }
    }
    return ref;
}
