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
struct mrg_idc_entry;
struct mrg_idc_file;

struct mrg_cmd;

enum mrg_mode {
  MRG_MODE_GAME,
  MRG_MODE_CONSOLE,
  MRG_MODE_MAPED,
};

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define MRG_TILE_W 16
#define MRG_TILE_H 16

#define MRG_DEBUG

#endif
