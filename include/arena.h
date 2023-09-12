#ifndef ARENA_H_
#define ARENA_H_

#include <stddef.h>
#include <stdint.h>

/**
 * An arena is a re-usable allocated buffer
 * Most functions that accept an arena will clear it before
 * using it and may call resize to ensure sufficent memory is allocated
 * Arenas are only meant as temporary containers and should not be used in
 * data structures with a long lifetime!
 * The primary purpose of an arena is to reduce malloc calls wherever possible
 * when temporary allocations are required.
 */

struct mrg_arena {
  uint8_t *data;
  size_t arena_len;

  size_t aptr;
};

struct mrg_arena mrg_arena_init(size_t len);

// resize the arena. this may invalidate previous allocations! use with caution!
int mrg_arena_resize(struct mrg_arena *arena, size_t len);

// malloc + resize
void *mrg_arena_mallocr(struct mrg_arena *arena, size_t len);
void *mrg_arena_malloc(struct mrg_arena *arena, size_t len);

void mrg_arena_clear(struct mrg_arena *arena);

void mrg_arena_free(struct mrg_arena *arena);

#endif
