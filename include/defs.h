#ifndef DEFS_H_
#define DEFS_H_

typedef struct mrg_platform mrg_platform;
struct mrg_state;
struct mrg_tile_set;
struct mrg_entity;
struct mrg_arena;

struct mrg_idc_header;
struct mrg_idc_dir;
struct mrg_idc_entity;
struct mrg_idc_room;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define MRG_DEBUG

#endif
