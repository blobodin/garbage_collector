#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdbool.h>
#include <stdio.h>

//// EVALUATION ERROR HANDLING ////

typedef enum {
    EXC_NONE = 0,

    EXC_SYNTAX_ERROR,
    EXC_NAME_ERROR,
    EXC_TYPE_ERROR,
    EXC_VALUE_ERROR,
    EXC_INDEX_ERROR,
    EXC_KEY_ERROR,
    EXC_MEMORY_ERROR,

    EXC_INTERNAL
} exception_t;

void exception_set(exception_t type, const char *message);
void exception_set_format(exception_t type, const char *format, ...);
void exception_clear(void);

void exception_print(FILE *stream);

exception_t exception_occurred();

#endif /* EXCEPTION_H */
