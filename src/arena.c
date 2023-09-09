#include "arena.h"
#include <stdlib.h>
#include <string.h>

struct mrg_arena mrg_arena_init(size_t len) {
  struct mrg_arena arena;
  memset(&arena, 0, sizeof(arena));
  arena.data = malloc(len);
  arena.arena_len = len;

  return arena;
}

int mrg_arena_resize(struct mrg_arena *arena, size_t len) {
  if (len < arena->arena_len) {
    return 0;
  }
  void *new_data = realloc(arena->data, len);
  if (new_data) {
    arena->data = new_data;
    arena->arena_len = len;
    return 0;
  }

  return -1;
}

void *mrg_arena_malloc(struct mrg_arena *arena, size_t len) {
  if (arena->aptr + len >= arena->arena_len) {
    return NULL;
  }

  void *data = arena->data + arena->aptr;

  arena->aptr += len;
  return data;
}

void mrg_arena_clear(struct mrg_arena *arena) { arena->aptr = 0; }

void mrg_arena_free(struct mrg_arena *arena) { free(arena->data); }
