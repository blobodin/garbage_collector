#include "parser.h"

#include "grammar.h"

static void parser_init(parser_t *obj, bool interactive, FILE *stream) {
    assert(obj != NULL);

    void *parser = yypstate_new();
    assert(parser != NULL);

    void *scanner;
    yylex_init_extra(obj, &scanner);
    assert(scanner != NULL);

    *obj = (parser_t) {
        .input = (parser_input_t) {
            .interactive = interactive,
            .stream = stream
        },
        .parser = parser,
        .scanner = scanner,
    };

    ast_init(&obj->ast);
}

static void parser_destroy(parser_t *d) {
    assert(d != NULL);

    /* Note that we do not destroy the AST here because it is expected
     * to remain available for evaluation. */
    free(d->input.buffer);
    yylex_destroy(d->scanner);
    yypstate_delete(d->parser);
}

parse_result_t parse(bool interactive, FILE *stream) {
    parser_t parser;
    parser_init(&parser, interactive, stream);

    int status = yylex(parser.scanner);
    parse_result_t result = {
        .type = status,
        .ast = parser.ast
    };

    parser_destroy(&parser);
    return result;
}

size_t parser_indent_cur(const parser_t *d) {
    return d->indent.stack[d->indent.pos];
}
void parser_indent_push(parser_t *d, size_t level) {
    assert(d->indent.pos + 1 < MAX_INDENT_LEVELS);
    d->indent.pos++;
    d->indent.stack[d->indent.pos] = level;
}
void parser_indent_pop(parser_t *d) {
    assert(d->indent.pos > 0);
    d->indent.pos--;
}

int parser_input_read(parser_input_t *input, char *buf, int *bytes, int len) {
    *bytes = 0;
    if (input->interactive) {
        if (input->position < input->length) {
            int avail = (int) input->length - input->position;
            int to_copy = avail > len ? len : avail;

            memcpy(buf, input->buffer + input->position, to_copy);
            input->position += to_copy;
            *bytes = to_copy;

            return 0;
        } else {
            return 1;
        }
    } else {
        *bytes = (int) fread(buf, 1, len, input->stream);
        return *bytes == 0 && feof(input->stream);
    }
}

int parser_input_wrap(parser_input_t *input) {
    if (interactive) {
        free(input->buffer);

        const char *prompt = input->buffer == NULL ? ">>> " : "... ";

#ifdef NREADLINE
        fprintf(stdout, "%s", prompt);

        input->buffer = NULL;
        size_t size;
        ssize_t len = getline(&input->buffer, &size, input->stream);

        if (len == -1) {
            return 1;
        }

        input->position = 0;
        input->length = len;
#else
        input->buffer = readline(prompt);

        if (input->buffer == NULL) {
            return 1;
        }

        size_t length = strlen(input->buffer);

        if (length) {
            /* Not a blank line, so record it in history. */
            add_history(input->buffer);

            /* This is wasteful; we'd prefer to just write the most recent
             * command to history, but append_history() isn't always
             * available. */
            write_history(SUBPYTHON_HISTORY);
        }

        input->buffer[length] = '\n';
        input->position = 0;
        input->length = length + 1;
#endif

        return 0;
    } else {
        return 1;
    }
}

void parse_result_destroy(parse_result_t *result) {
    assert(result != NULL);
    ast_destroy(&result->ast);
}
