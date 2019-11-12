#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>


/*!
 * An opaque reference that can be used to indirectly refer to a value_t.
 * The reference itself is not a pointer; rather, it is an index into a table of
 * references maintained by alloc.c. Use deref() to get the value_t pointer.
 */
typedef int32_t reference_t;


/*!
 * This value is used to represent a "null" or invalid reference.
 * Note that this is different than a reference to None.
 * A reference to None is a real reference with a nonnegative value.
 */
#define NULL_REF ((reference_t) (-1))

/*!
 * An invalid reference that is used to mark dict key slots that were deleted.
 * Linear probing will continue past a TOMBSTONE_REF,
 * so we don't need to rearrange dict elements during deletion.
 */
#define TOMBSTONE_REF ((reference_t) (-2))


/*!
 * An enumeration of all types of values supported by the interpreter.
 */
typedef enum {
    VAL_NONE,           /*!< The None value. There is only one! */
    VAL_BOOL,           /*!< A bool value. There are only True and False! */
    VAL_INTEGER,        /*!< An integer value. */
    VAL_STRING,         /*!< A string value. */
    VAL_LIST,           /*!< A list value. */
    VAL_DICT,           /*!< A dictionary value. */

    NUM_TYPES,          /*!< The number of different value types. */

    VAL_REF_ARRAY,      /*!< A value used internally to store an array of references. */

    VAL_FREE            /*!< Used to indicate that a slot in the memory pool is free. */
} value_type_t;

/*!
 * A value_t type common to all values used within the interpreter.
 * If a value has additional data, it is stored immediately after this struct.
 *
 *  - None and bool types do not contain any data and therefore just use value_t
 *  - Integers are 64-bit and use integer_value_t
 *  - Strings use string_value_t and are '\0'-terminated
 *  - Lists (list_value_t) are represented as fixed-length arrays of references,
 *    stored in a ref_array_value_t that is allocated from the memory pool
 *  - Dictionaries (dict_value_t) use a linear probing hash table.
 *    Both keys and values are stored as arrays of references,
 *    using ref_array_value_t structs allocated from the memory pool.
 */
typedef struct {
    /*! This specifies what kind of value is actually represented. */
    value_type_t type;

    /*!
     * The number of places this value is currently referenced.
     * Every time a new value refers to this value, ref_count is incremented.
     * Every time a value referring to this value is destroyed
     * or set to a different value, ref_count should be decremented.
     * If ref_count == 0, this value is definitely garbage and should be freed.
     */
    size_t ref_count;

    /*!
     * The number of bytes occupied by the value, including any additional payload.
     * Some types are a fixed size and will always set this member to the same value.
     * For strings, this is sizeof(string_value_t) + strlen(string_value) + 1.
     * This value is rounded up to a multiple of 8 so that values are aligned.
     */
    size_t value_size;
} value_t;

/*!
 * A "integer value" type that represents long integers.
 * If the type of a value_t* is VAL_INTEGER,
 * it can be cast to an integer_value_t*.
 */
typedef struct {
    value_t base;

    /*! The integer value this integer_value_t represents. */
    int64_t integer_value;
} integer_value_t;

/*!
 * A "string value" type that represents strings.
 * If the type of a value_t* is VAL_STRING, it can be cast to a string_value_t*.
 */
typedef struct {
    value_t base;

    /*!
     * The string value this string_value_t represents.
     * The characters are stored immediately following the value_t struct.
     * The string is '\0'-terminated.
     */
    char string_value[];
} string_value_t;


/*!
 * A "list value" type that represents list elements.
 * If the type of a value_t* is VAL_LIST, it can be cast to a list_value_t*.
 */
typedef struct {
    value_t base;

    /*!
     * The number of elements currently stored in the list.
     * This can be less than deref(values)->capacity if elements were deleted.
     */
    int64_t size;

    /*!
     * The reference to the ref_array_value_t of values stored in this list.
     */
    reference_t values;
} list_value_t;


/*!
 * A "dictionary value" type that represents dictionaries.
 * If the type of a value_t* is VAL_DICT, it can be cast to a dict_value_t*.
 */
typedef struct {
    value_t base;

    /*!
     * The number of elements currently stored in the dict.
     */
    int64_t size;

    /*!
     * The number of key slots currently occupied in the dict.
     * This can be greater than size if there are tombstones.
     */
    int64_t occupied;

    /*!
     * The reference to the ref_array_value_t containing the dictionary's keys.
     * Used to implement O(1) lookup.
     * Deleting keys from dictionaries sets their slots to TOMBSTONE_REF,
     * so TOMBSTONE_REF should be treated as NULL_REF in the reference graph.
     */
    reference_t keys;

    /*!
     * The reference to the ref_array_value_t containing the dictionary's values.
     */
    reference_t values;
} dict_value_t;

/*!
 * A "ref array" value that is used within the evaluator to hold lists of
 * reference. This is used by list_value_t to store the elements of the list
 * and by dict_value_t to store the both the lists of key and values.
 * If the type of a value_t* is VAL_REF_ARRAY,
 * it can be cast to a ref_array_value_t*.
 */
typedef struct {
    value_t base;

    /*!
     * This is the number of references that can be stored in this array.
     */
    size_t capacity;

    /*!
     * The array of references, stored immediately following the value_t struct.
     */
    reference_t values[];
} ref_array_value_t;

#endif /* TYPES_H */
