#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "arena.h"
#include "ast.h"

#define MAX_INDENT_LEVELS 100

typedef struct parser_location_t {
    int first_line;
    int first_column;
    int last_line;
    int last_column;
} parser_location_t;

typedef struct parser_input_t {
    bool interactive;
    FILE *stream;
    char *buffer;
    size_t length;
    size_t position;
} parser_input_t;

typedef struct parser_t {
    parser_input_t input;

    void *scanner;
    void *parser;

    ast_t ast;
    parser_location_t error_location;

    struct {
        size_t stack[MAX_INDENT_LEVELS];
        size_t pos;
    } indent;
} parser_t;

typedef enum parse_result_type_t {
    RESULT_SUCCESS,
    RESULT_FAILED,
    RESULT_OOM,
    RESULT_EXIT
} parse_result_type_t;

typedef struct parse_result_t {
    parse_result_type_t type;
    ast_t ast;
} parse_result_t;

parse_result_t parse(bool interactive, FILE *file);

size_t parser_indent_cur(const parser_t *d);
void parser_indent_push(parser_t *d, size_t level);
void parser_indent_pop(parser_t *d);

int parser_input_read(parser_input_t *input, char *buf, int *bytes, int len);
int parser_input_wrap(parser_input_t *input);

void parse_result_destroy(parse_result_t *result);

#endif /* PARSER_H */
