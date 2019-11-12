#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

typedef struct arena_t arena_t;

arena_t *arena_new();
void arena_free(arena_t *arena);

void *arena_malloc(arena_t *arena, size_t size);
void *arena_strdup(arena_t *arena, const char *str);

#endif /* ARENA_H */
