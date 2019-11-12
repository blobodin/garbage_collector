#include "eval_dict.h"

#include <assert.h>
#include "eval_refs.h"
#include "eval_types.h"
#include "exception.h"
#include "refs.h"

static inline dict_value_t *dict_coerce(value_t *obj) {
    assert(obj->type == VAL_DICT);
    return (dict_value_t *) obj;
}

static inline ref_array_value_t *dict_keyarray(dict_value_t *dict) {
    value_t *obj = deref(dict->keys);
    assert(obj->type == VAL_REF_ARRAY);
    return (ref_array_value_t *) obj;
}
static inline ref_array_value_t *dict_valuearray(dict_value_t *dict) {
    value_t *obj = deref(dict->values);
    assert(obj->type == VAL_REF_ARRAY);
    return (ref_array_value_t *) obj;
}

static int64_t keys_find(ref_array_value_t *keys, uint64_t hash, reference_t subscr, bool skip_tombstones) {
    int64_t capacity = keys->capacity;
    int64_t bucket = hash % capacity;

    for (int64_t idx = 0; idx < capacity; idx++) {
        int64_t current = bucket + idx;
        if (current > capacity) {
            current -= capacity;
        }

        reference_t key = keys->values[current];
        if (skip_tombstones && key == TOMBSTONE_REF) {
            continue;
        }
        if (key == NULL_REF || key == TOMBSTONE_REF || ref_eq(subscr, key)) {
            return current;
        }
    }

    return -1;
}

static void dict_upsize(dict_value_t *dict) {
    ref_array_value_t *keys = dict_keyarray(dict);
    ref_array_value_t *values = dict_valuearray(dict);
    int64_t capacity = keys->capacity;

    int64_t new_capacity = capacity * 2;
    reference_t ref_keys = make_reference_refarray(new_capacity);
    if (exception_occurred()) {
        return;
    }
    reference_t ref_values = make_reference_refarray(new_capacity);
    if (exception_occurred()) {
        decref(ref_keys);
        return;
    }

    ref_array_value_t *new_keys = (ref_array_value_t *) deref(ref_keys);
    ref_array_value_t *new_values = (ref_array_value_t *) deref(ref_values);

    for (int64_t idx = 0; idx < capacity; idx++) {
        if (keys->values[idx] != TOMBSTONE_REF && keys->values[idx] != NULL_REF) {
            reference_t key = keys->values[idx];
            reference_t value = values->values[idx];

            uint64_t hash = ref_hash(key);
            if (exception_occurred()) {
                decref(ref_keys);
                decref(ref_values);
                return;
            }
            int64_t idx = keys_find(new_keys, hash, key, false);

            /* Increment the counts for the key and value because the new
             * reference arrays now refer to them. */
            incref(key);
            incref(value);
            new_keys->values[idx] = key;
            new_values->values[idx] = value;
        }
    }

    /* Now replace the old key and value arrays. */
    decref(dict->keys);
    decref(dict->values);
    dict->keys = ref_keys;
    dict->values = ref_values;

    /* This process removes all tombstones, so the number of occupied slots
     * become equal to the number of elements in the dictionary. */
    dict->occupied = dict->size;
}
static void dict_maybe_upsize(dict_value_t *dict) {
    int64_t capacity = dict_keyarray(dict)->capacity;
    if (dict->occupied * 2 >= capacity) {
        dict_upsize(dict);
    }
}

bool dict_bool(value_t *obj) {
    return dict_len(obj) > 0;
}

int64_t dict_len(value_t *obj) {
    return dict_coerce(obj)->size;
}

reference_t dict_subscr_get(value_t *obj, reference_t subscr) {
    dict_value_t *dict = dict_coerce(obj);
    ref_array_value_t *keys = dict_keyarray(dict);
    ref_array_value_t *values = dict_valuearray(dict);

    uint64_t hash = ref_hash(subscr);
    if (exception_occurred()) {
        return NULL_REF;
    }

    int64_t idx = keys_find(keys, hash, subscr, true);

    if (idx >= 0 && keys->values[idx] != NULL_REF) {
        return values->values[idx];
    } else {
        exception_set(EXC_KEY_ERROR, "no value found for key in dictionary");
        return NULL_REF;
    }
}

void dict_subscr_set(value_t *obj, reference_t subscr, reference_t value) {
    dict_value_t *dict = dict_coerce(obj);
    ref_array_value_t *keys = dict_keyarray(dict);
    ref_array_value_t *values = dict_valuearray(dict);

    uint64_t hash = ref_hash(subscr);
    if (exception_occurred()) {
        return;
    }

    /* Look for an existing entry with this key. */
    int64_t idx = keys_find(keys, hash, subscr, true);
    if (idx >= 0 && keys->values[idx] != NULL_REF) {
        /* If the keys match, just overwrite the value. */
        values->values[idx] = value;
        return;
    }

    /* Otherwise, look for a new slot to set. */
    idx = keys_find(keys, hash, subscr, false);
    if (idx >= 0) {
        assert(keys->values[idx] == NULL_REF || keys->values[idx] == TOMBSTONE_REF);
        dict->size++;
        if (keys->values[idx] == NULL_REF) {
            dict->occupied++;
        }

        keys->values[idx] = subscr;
        values->values[idx] = value;

        dict_maybe_upsize(dict);
    } else {
        dict_upsize(dict);
        dict_subscr_set(obj, subscr, value);
    }
}

void dict_subscr_del(value_t *obj, reference_t subscr) {
    dict_value_t *dict = dict_coerce(obj);
    ref_array_value_t *keys = dict_keyarray(dict);
    ref_array_value_t *values = dict_valuearray(dict);

    uint64_t hash = ref_hash(subscr);
    if (exception_occurred()) {
        return;
    }

    int64_t idx = keys_find(keys, hash, subscr, true);
    if (idx >= 0 && keys->values[idx] != NULL_REF) {
        dict->size--;
        keys->values[idx] = TOMBSTONE_REF;
        values->values[idx] = NULL_REF;
    } else {
        exception_set(EXC_KEY_ERROR, "can't delete nonexistant key in dictionary");
    }
}

bool dict_eq(value_t *l, value_t *r) {
    dict_value_t *ldict = dict_coerce(l);
    dict_value_t *rdict = dict_coerce(r);

    if (ldict->size != rdict->size) {
        return false;
    }

    ref_array_value_t *lkeys = dict_keyarray(ldict);
    ref_array_value_t *lvalues = dict_valuearray(ldict);
    ref_array_value_t *rkeys = dict_keyarray(rdict);
    ref_array_value_t *rvalues = dict_valuearray(rdict);

    for (size_t idx = 0; idx < lkeys->capacity; idx++) {
        reference_t key = lkeys->values[idx];
        if (key == NULL_REF || key == TOMBSTONE_REF) {
            continue;
        }

        int64_t ridx = keys_find(rkeys, ref_hash(key), key, true);
        bool equals = ridx >= 0 && rkeys->values[ridx] != NULL_REF &&
            ref_eq(lvalues->values[idx], rvalues->values[ridx]);
        if (exception_occurred() || !equals) {
            return false;
        }
    }

    return true;
}

void dict_print(value_t *obj, FILE *stream, size_t depth) {
    /* If we have reached the maximum recursion depth, print a placeholder. */
    if (depth == 0) {
        fprintf(stream, "...");
        return;
    }

    /* Then make sure that we are dealing with a dictionary. */
    dict_value_t *dict = dict_coerce(obj);
    ref_array_value_t *keys = dict_keyarray(dict);
    ref_array_value_t *values = dict_valuearray(dict);

    bool comma = false;
    fprintf(stream, "{");
    for (size_t idx = 0; idx < keys->capacity; idx++) {
        reference_t key = keys->values[idx];
        if (key == NULL_REF || key == TOMBSTONE_REF) {
            continue;
        }

        if (comma) {
            fprintf(stream, ", ");
        }

        ref_print_repr(key, stream, depth - 1);
        fprintf(stream, ": ");
        ref_print_repr(values->values[idx], stream, depth - 1);

        comma = true;
    }
    fprintf(stream, "}");
}
