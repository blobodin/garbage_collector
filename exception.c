#include "exception.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INITIAL_FORMAT_BUFSIZE 256

//// GLOBAL ERROR STATE ////

static struct {
    exception_t type;
    char *error;
} exception;

//// EVALUATION ERROR HANDLING ////

void exception_set(exception_t type, const char *message) {
    free(exception.error);

    exception.type = type;
    exception.error = strdup(message);
}

void exception_set_format(exception_t type, const char *format, ...) {
    char initial_buf[INITIAL_FORMAT_BUFSIZE];
    va_list args;

    va_start(args, format);
    int result = vsnprintf(initial_buf, INITIAL_FORMAT_BUFSIZE, format, args);
    va_end(args);

    if (result < 0) {
        fprintf(stderr, "failed to format exception message with format '%s'\n", format);
        abort();
    }

    /* Here, we use malloc and not a string object because we don't want an
     * exception caused by running out of regular memory to require a new
     * value to be allocated. */
    char *buf = malloc(result + 1);
    if (buf == NULL) {
        fprintf(stderr, "failed to allocate exception message buffer\n");
        abort();
    }

    if (result <= INITIAL_FORMAT_BUFSIZE) {
        memcpy(buf, initial_buf, result);
        buf[result] = '\0';
    } else {
        va_start(args, format);
        vsnprintf(buf, result + 1, format, args);
        va_end(args);
    }

    exception.type = type;
    exception.error = buf;
}
void exception_clear(void) {
    free(exception.error);

    exception.type = EXC_NONE;
    exception.error = NULL;
}

static const char *exception_type_to_str(exception_t type) {
    switch (type) {
        case EXC_NONE:          return "<none>";
        case EXC_SYNTAX_ERROR:  return "SyntaxError";
        case EXC_NAME_ERROR:    return "NameError";
        case EXC_TYPE_ERROR:    return "TypeError";
        case EXC_VALUE_ERROR:   return "ValueError";
        case EXC_INDEX_ERROR:   return "IndexError";
        case EXC_KEY_ERROR:     return "KeyError";
        case EXC_MEMORY_ERROR:  return "MemoryError";
        case EXC_INTERNAL:      return "<internal>";
    }
    return "<unknown>";
}
void exception_print(FILE *stream) {
    if (exception.type) {
        if (exception.error) {
            fprintf(stream, "%s: %s\n", exception_type_to_str(exception.type), exception.error);
        } else {
            fputs(exception_type_to_str(exception.type), stream);
        }
    }
}

exception_t exception_occurred() {
    return exception.type;
}
