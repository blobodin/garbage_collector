/*! \file
 * Manages references to values allocated in a memory pool.
 * Implements reference counting and garbage collection.
 *
 * Adapted from Andre DeHon's CS24 2004, 2006 material.
 * Copyright (C) California Institute of Technology, 2004-2010.
 * All rights reserved.
 */

#include "refs.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "eval.h"
#include "mm.h"

/*! The alignment of value_t structs in the memory pool. */
#define ALIGNMENT 8


//// MODULE-LOCAL STATE ////

/* The start of the from pool for stop and copy. */
void *pool;

/* Half the size of the total memory pool. */
size_t half_mem_size;

/* The start of the to pool for stop and copy. */
void *to_pool;

/*!
 * This is the "reference table", which maps references to value_t pointers.
 * The value at index i is the location of the value_t with reference i.
 * An unused reference is indicated by storing NULL as the value_t*.
 */
static value_t **ref_table;

/*!
 * This is the number of references currently in the table, including unused ones.
 * Valid entries are in the range 0 .. num_refs - 1.
 */
static reference_t num_refs;

/*!
 * This is the maximum size of the ref_table.
 * If the table grows larger, it must be reallocated.
 */
static reference_t max_refs;


//// FUNCTION DEFINITIONS ////


/*!
 * This function initializes the references and the memory pool.
 * It must be called before allocations can be served.
 */
void init_refs(size_t memory_size, void *memory_pool) {
    /* Use the memory pool of the given size.
     * We round the size down to a multiple of ALIGNMENT so that values are aligned.
     */

    /* Cuts the memory pool in half for the from pool and the to pool. */
    half_mem_size = memory_size / 2;
    to_pool = memory_pool + half_mem_size;

    /* Initializes the first from pool. */
    mm_init((half_mem_size / ALIGNMENT) * ALIGNMENT, memory_pool);
    pool = memory_pool;

    /* Start out with no references in our reference-table. */
    ref_table = NULL;
    num_refs = 0;
    max_refs = 0;
}


/*! Allocates an available reference in the ref_table. */
static reference_t assign_reference(value_t *value) {
    /* Scan through the reference table to see if we have any unused slots
     * that can store this value. */
    for (reference_t i = 0; i < num_refs; i++) {
        if (ref_table[i] == NULL) {
            ref_table[i] = value;
            return i;
        }
    }

    /* If we are out of slots, increase the size of the reference table. */
    if (num_refs == max_refs) {
        /* Double the size of the reference table, unless it was 0 before. */
        max_refs = max_refs == 0 ? INITIAL_SIZE : max_refs * 2;
        ref_table = realloc(ref_table, sizeof(value_t *[max_refs]));
        if (ref_table == NULL) {
            fprintf(stderr, "could not resize reference table");
            exit(1);
        }
    }

    /* No existing references were unused, so use the next available one. */
    reference_t ref = num_refs;
    num_refs++;
    ref_table[ref] = value;
    return ref;
}


/*! Attempts to allocate a value from the memory pool and assign it a reference. */
reference_t make_ref(value_type_t type, size_t size) {
    /* Force alignment of data size to ALIGNMENT. */
    size = (size + ALIGNMENT - 1) / ALIGNMENT * ALIGNMENT;

    /* Find a (free) location to store the value. */
    value_t *value = mm_malloc(size);

    /* If there was no space, then fail. */
    if (value == NULL) {
        return NULL_REF;
    }

    /* Initialize the value. */
    assert(value->type == VAL_FREE);
    value->type = type;
    value->ref_count = 1; // this is the first reference to the value

    /* Set the data area to a pattern so that it's easier to debug. */
    memset(value + 1, 0xCC, value->value_size - sizeof(value_t));

    /* Assign a reference_t to it. */
    return assign_reference(value);
}


/*! Dereferences a reference_t into a pointer to the underlying value_t. */
value_t *deref(reference_t ref) {
    /* Make sure the reference is actually a valid index. */
    assert(ref >= 0 && ref < num_refs);

    value_t *value = ref_table[ref];

    /* Make sure the reference's value is within the pool!
     * Also ensure that the value is not NULL, indicating an unused reference. */
    // assert(is_pool_address(value));

    return value;
}

/*! Returns the reference that maps to the given value. */
reference_t get_ref(value_t *value) {
    for (reference_t i = 0; i < num_refs; i++) {
        if (ref_table[i] == value) {
            return i;
        }
    }
    assert(!"Value has no reference");
}


/*! Returns the number of values in the memory pool. */
size_t refs_used() {
    size_t values = 0;
    for (reference_t i = 0; i < num_refs; i++) {
        if (ref_table[i] != NULL) {
            values++;
        }
    }
    return values;
}

/*!
 * General function that recursivley iterates through all references that a
 * value is associated with and applies a given function. In this case,
 * the function f with either be move() or decref().
 */
void apply_to_neighbors(void (*f)(reference_t ref), value_t *val) {
    if (val->type == VAL_LIST) {
        list_value_t *list = (list_value_t *) val;
        f(list->values);
    } else if (val->type == VAL_DICT) {
        dict_value_t *dict = (dict_value_t *) val;
        f(dict->keys);
        f(dict->values);
    } else if (val->type == VAL_REF_ARRAY) {
        ref_array_value_t *ref_array = (ref_array_value_t *) val;
        size_t array_size = ref_array->capacity;
        for (size_t i = 0; i < array_size; i++) {
            f(ref_array->values[i]);
        }
    }
}

//// REFERENCE COUNTING ////

/*! Increases the reference count of the value at the given reference. */
void incref(reference_t ref) {
    value_t *val = deref(ref);
    val->ref_count++;
}


/*!
 * Decreases the reference count of the value at the given reference.
 * If the reference count reaches 0, the value is definitely
 * garbage and should be freed. Additionally, all things referenced to
 * by the value must be decrefed once its freed.
 */
void decref(reference_t ref) {
    if (ref != TOMBSTONE_REF && ref != NULL_REF) {
        value_t *val = deref(ref);
        val->ref_count--;
        if (val->ref_count == 0) {
            apply_to_neighbors(decref, val);
            mm_free(val);
            ref_table[ref] = NULL;
        }
    }
}


//// END REFERENCE COUNTING ////

//// GARBAGE COLLECTOR ////

/*!
 * Function for moving references between the from space and to space.
 * Updates reference table and reference counters accordingly.
 */
void move(reference_t ref) {
    if (ref != NULL_REF && ref != TOMBSTONE_REF) {
        value_t *val = deref(ref);
        /* If the value hasn't been moved, move it. */
        if (!is_pool_address(val)) {
            /* Reset the reference count. */
            val->ref_count = 1;
            /* Get memory in the to space and copy the value over. */
            void *ptr = mm_malloc(val->value_size);
            void *new_ptr = memcpy(ptr, (void *) val, val->value_size);
            /* Update the reference table address. */
            ref_table[ref] = new_ptr;
            /* Attempt to recrusively move all neighbors. */
            apply_to_neighbors(move, val);
        } else {
            /* If the value has been moved, increment the reference count. */
            val->ref_count++;
        }
    }
}

/*
 * stop_and_copy's only function is to call move. This is so that move()
 * and decref() both have the same arguments and can be used by the
 * general function apply_to_neighbors().
 */
void stop_and_copy(const char *name, reference_t ref) {
    (void) name;
    move(ref);
}

/*!
 * Iterates through the reference table to collected garbage cycles
 * that have not been moved to the to_space.
 */
void clean_cycles() {
    for (int i = 0; i < num_refs; i++) {
        if (!is_pool_address(ref_table[i])) {
            ref_table[i] = NULL;
        }
    }
}

void collect_garbage(void) {
    if (interactive) {
        fprintf(stderr, "Collecting garbage.\n");
    }
    size_t old_use = mem_used();

    /* Initalizes to_space memory so that it can be malloced */
    mm_init((half_mem_size / ALIGNMENT) * ALIGNMENT, to_pool);

    /* Copies over all values referenced to by global variables. */
    foreach_global(stop_and_copy);

    /* Removes cycles of garbage not caught by reference counting */
    clean_cycles();

    /* Switches from space and to space pointers after all garbage is collected
       and all used memory is copied over. */
    void *pool_storage = pool;
    pool = to_pool;
    to_pool = pool_storage;

    if (interactive) {
        /* This will report how many bytes we were able to free in this garbage
           collection pass. */
        fprintf(stderr, "Reclaimed %zu bytes of garbage.\n", old_use - mem_used());
    }
}

//// END GARBAGE COLLECTOR ////

/*!
 * Clean up the allocator state.
 * This requires freeing the memory pool and the reference table,
 * so that the allocator doesn't leak memory.
 */
void close_refs(void) {
    /* Determines which global pointer points to the beginning of the
       memory pool. */
    if ((size_t) pool < (size_t) to_pool) {
        mm_free(pool);
    } else {
        mm_free(to_pool);
    }
    free(ref_table);
}
