/*! \file
 * Manages references to values allocated in a memory pool.
 * Declares functions for reference counting and garbage collection.
 *
 * Adapted from Andre DeHon's CS24 2004, 2006 material.
 * Copyright (C) California Institute of Technology, 2004-2009.
 * All rights reserved.
 */

#ifndef REFS_H
#define REFS_H

#include "types.h"


/* Initializes the references and the memory pool state. */
void init_refs(size_t memory_size, void *memory_pool);

/* Attempts to allocate a value from the memory pool and assign it a reference. */
reference_t make_ref(value_type_t type, size_t size);

/* Dereference a reference_t into its corresponding value_t. */
value_t *deref(reference_t ref);

/*!
 * Returns the reference that maps to the given value. This is the inverse of deref().
 * This function is very slow; use for debugging only!
 */
reference_t get_ref(value_t *value);

/* Returns the number of values in the pool. */
size_t refs_used(void);

/* Increases the reference count of the value at the given reference. */
void incref(reference_t ref);

/* Decreases the reference count of the value at the given reference. */
void decref(reference_t ref);

/* Runs the garbage collector to reclaim unused space. */
void collect_garbage(void);

/* Clean up the allocator and memory pool state. */
void close_refs(void);


#endif /* REFS_H */
