#ifndef MM_H
#define MM_H

#include <stdbool.h>
#include <stddef.h>
#include "types.h"

/*! Initializes a memory pool in the given region of the given size in bytes. */
void mm_init(size_t size, void *pool);

/*!
 * Allocates and returns a block of the given size.
 * Returns NULL and sets a subpython exception if out of memory.
 */
value_t *mm_malloc(size_t size);

/*!
 * Adds a value to the free list so it can be used for future allocations.
 * The value's type is also set to VAL_FREE.
 */
void mm_free(value_t *value);

/*! Returns whether the specified address is within the memory pool. */
bool is_pool_address(void *addr);

/*! Returns the number of bytes of used memory. */
size_t mem_used(void);

/*! Prints all allocated objects and free regions in the pool. */
void mem_dump(void);

#endif /* MM_H */
