/*! \file
 * Implements a simple memory allocator for a region of memory.
 * The allocator uses a singly-linked explicit free list
 * and can perform splits but not coalesces.
 */

#include "mm.h"

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>

#include "refs.h"
#include "eval.h"
#include "exception.h"

/*! The number of bytes in the memory pool. */
static size_t memory_size;

/*! The start of the memory pool. */
static uint8_t *memory_pool;

/*!
 * The payloads of free values, used to construct an explicit free list.
 * The allocator performs splits but not coalesces,
 * so the free list doesn't need to be doubly-linked.
 * The allocator tries to satisfy requests with the best-fit block in the list,
 * and expands the end of the heap if no block is large enough.
 * Hopefully this looks familiar from the malloc assignment!
 */
typedef struct free_value free_value_t;
struct free_value {
    /*! Always set to VAL_FREE. */
    value_type_t type;

    /*!
     * The next free_value_t in the free list, or NULL if this is the last.
     * Note that this field replaces the ref_count field in the value_t struct,
     * since free values don't have reference counts.
     */
    free_value_t *next;

    /*! The number of free bytes in this block. */
    size_t value_size;
};

/*! The head of the free list, or NULL if it is empty. */
static free_value_t *free_list;

void mm_init(size_t size, void *pool) {
    memory_size = size;
    memory_pool = pool;

    /* Make the entire pool a free value. */
    free_list = pool;
    free_list->type = VAL_FREE;
    free_list->next = NULL;
    free_list->value_size = size;
}

value_t *mm_malloc(size_t size) {
    /* Use a best-fit algorithm to search for a free value to use. */
    free_value_t **best_fit = NULL;
    size_t smallest_size = SIZE_MAX;
    for (
        free_value_t **free_value = &free_list;
        *free_value != NULL;
        free_value = &(*free_value)->next
    ) {
        size_t free_size = (*free_value)->value_size;
        if (free_size >= size && free_size < smallest_size) {
            smallest_size = free_size;
            best_fit = free_value;
        }
    }
    /* Report an error if there is no remaining space. */
    if (best_fit == NULL) {
        exception_set_format(EXC_MEMORY_ERROR,
                "cannot service request of size %zu with %zu bytes allocated",
                size, mem_used());
        return NULL;
    }

    value_t *value = (value_t *) *best_fit;
    size_t remaining_size = smallest_size - size;
    if (remaining_size > sizeof(value_t)) {
        /* Split the free value if there is enough space for another value. */
        value->value_size = size;
        free_value_t *next = (free_value_t *) ((uint8_t *) value + size);
        next->type = VAL_FREE;
        next->next = (*best_fit)->next;
        next->value_size = remaining_size;
        *best_fit = next;
    }
    else {
        /* Otherwise, just remove this value from the free list. */
        *best_fit = (*best_fit)->next;
    }
    /* Return the best-fit block. */
    return value;
}

void mm_free(value_t *value) {
    value->type = VAL_FREE;
    free_value_t *free_value = (free_value_t *) value;
    free_value->next = free_list;
    free_list = free_value;

    /* Set the data area to a pattern so that it's easier to debug. */
    memset(value + 1, 0xCC, value->value_size - sizeof(value_t));
}

bool is_pool_address(void *addr) {
    return (uint8_t *) addr >= memory_pool &&
           (uint8_t *) addr <  memory_pool + memory_size;
}

size_t mem_used() {
    /* Subtract each free block from the total memory size. */
    size_t used = memory_size;
    for (
        free_value_t *free_value = free_list;
        free_value != NULL;
        free_value = free_value->next
    ) {
        used -= free_value->value_size;
    }
    return used;
}

void mem_dump() {
    for (size_t offset = 0, value_size; offset < memory_size; offset += value_size) {
        value_t *value = (value_t *) (memory_pool + offset);
        value_size = value->value_size;

        /* If this is a free value, continue to the next one. */
        if (value->type == VAL_FREE) {
            fprintf(stdout, "Free  0x%08zx; size %zu\n", offset, value_size);
            continue;
        }

        reference_t ref = get_ref(value);
        fprintf(stdout, "Value 0x%08zx; size %zu; ref %d; refcnt: %zu; ",
            offset, value_size, ref, value->ref_count);

        switch (value->type) {
            case VAL_NONE:
                fprintf(stdout, "type = VAL_NONE; value = None\n");
                break;

            case VAL_BOOL:
                fprintf(stdout, "type = VAL_BOOL; value = %s\n",
                            ref_is_true(ref) ? "True" : "False");
                break;

            case VAL_INTEGER:
                fprintf(stdout, "type = VAL_INTEGER: value = %" PRIi64 "\n",
                    ((integer_value_t *) value)->integer_value);
                break;

            case VAL_STRING:
                fprintf(stdout, "type = VAL_STRING; value = \"%s\"\n",
                    ((string_value_t *) value)->string_value);
                break;

            case VAL_LIST:
                fprintf(stdout, "type = VAL_LIST; values = %d\n",
                    ((list_value_t *) value)->values);
                break;

            case VAL_DICT: {
                dict_value_t *dict = (dict_value_t *) value;
                fprintf(stdout,
                    "type = VAL_DICT; keys = %d; values = %d\n",
                    dict->keys, dict->values);
                break;
            }

            case VAL_REF_ARRAY: {
                ref_array_value_t *rav = (ref_array_value_t *) value;
                fprintf(stdout, "type = VAL_REF_ARRAY; values = [");
                for (size_t i = 0; i < rav->capacity; i++) {
                    if (i > 0) {
                        fprintf(stdout, ", ");
                    }
                    fprintf(stdout, "%d", rav->values[i]);
                }
                fprintf(stdout, "]\n");
                break;
            }

            default:
                fprintf(stdout,
                        "type = UNKNOWN; the memory pool is probably corrupt\n");
        }
    }
}
