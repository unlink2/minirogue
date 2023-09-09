#ifndef ARENA_H_
#define ARENA_H_

#include <stddef.h>
#include <stdint.h>

struct mrg_arena {
  uint8_t *data;
  size_t arena_len;

  size_t aptr;
};

struct mrg_arena mrg_arena_init(size_t len);

int mrg_arena_resize(struct mrg_arena *arena, size_t len);

void* mrg_arena_malloc(struct mrg_arena *arena, size_t len);

void mrg_arena_clear(struct mrg_arena *arena);

void mrg_arena_free(struct mrg_arena *arena);

#endif 
