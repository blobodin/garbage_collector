#include "arena.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_BLOCK_SIZE 8192
#define ALIGNMENT 8

typedef struct arena_block_t {
    size_t size;
    size_t allocated;
    struct arena_block_t *prev;
    void *area;
} arena_block_t;

static arena_block_t *block_new(size_t size, arena_block_t *prev) {
    arena_block_t *block = malloc(sizeof(arena_block_t));
    void *area = malloc(size);

    assert(block != NULL);
    assert(area != NULL);

    *block = (arena_block_t) {
        .size = size,
        .allocated = 0,
        .prev = prev,
        .area = area
    };

    return block;
}

static void block_free(arena_block_t *block) {
    if (block) {
        free(block->area);
        free(block);
    }
}

typedef struct arena_t {
    arena_block_t *current;
} arena_t;

arena_t *arena_new() {
    arena_t *arena = calloc(1, sizeof(arena_t));
    arena_block_t *block = block_new(DEFAULT_BLOCK_SIZE, NULL);

    assert(arena != NULL);
    assert(block != NULL);

    *arena = (arena_t) {
        .current = block
    };

    return arena;
}

void arena_free(arena_t *arena) {
    if (arena) {
        arena_block_t *block = arena->current;
        while (block) {
            arena_block_t *temp = block->prev;
            block_free(block);
            block = temp;
        }
        free(arena);
    }
}

void *arena_malloc(arena_t *arena, size_t size) {
    assert(arena != NULL);

    /* Round up size to required alignment. */
    size = (size + ALIGNMENT - 1) / ALIGNMENT * ALIGNMENT;

    if (arena->current->allocated + size > arena->current->size) {
        /* Need more memory in the arena. */
        size_t block_size = size > DEFAULT_BLOCK_SIZE ? size : DEFAULT_BLOCK_SIZE;
        arena->current = block_new(block_size, arena->current);
    }

    void *start = (char *) arena->current->area + arena->current->allocated;
    arena->current->allocated += size;
    return start;
}


void *arena_strdup(arena_t *arena, const char *str) {
    size_t len = strlen(str);
    char *new = arena_malloc(arena, len + 1);
    if (new) {
        new[len] = '\0';
        memcpy(new, str, len);
    }
    return new;
}
