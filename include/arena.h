#ifndef ARENA_H_
#define ARENA_H_

#include <stddef.h>
#include <stdint.h>

struct mr_arena {
  uint8_t *data;
  size_t arena_len;

  size_t aptr;
};

struct mr_arena mr_arena_init(size_t len);

void* mr_arena_malloc(struct mr_arena *arena, size_t len);

void mr_arena_clear(struct mr_arena *arena);

void mr_arena_free(struct mr_arena *arena);

#endif 
