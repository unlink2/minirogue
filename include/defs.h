#ifndef DEFS_H_
#define DEFS_H_

#include <stdint.h>
#include "fxp.h"

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

/**
 * Room defs
 */

#define MRG_IDC_FILE_NAME_LEN 8

enum mrg_room_flags { MRG_ROOM_ALLOC = 1 };

/**
 * Room definition
 * n_entities: amount of entities to load
 * entities_offset: location of entity list in file
 * room_w: room width
 * room_h: room height
 * tile_offset: location of tile map for the room (load room_w * room_h)
 * bytes) flags_offset: location of flag map for the room (load room_w * room_h
 * bytes)
 * tile_set_handle: see note on tile set loading
 */
struct mrg_room {
  int32_t room_id;
  int32_t room_w;
  int32_t room_h;
  int32_t tiles_offset;
  int32_t flags_offset;
  char tile_set[MRG_IDC_FILE_NAME_LEN];
  int8_t *tiles;
  int8_t *flags;

  int ok;

  int tileset_id;
  enum mrg_room_flags iflags;
};

/**
 * Entity defs
 */
#define MRG_ENTITY_SLOTS_MAX 128

enum mrg_entities {
  MRG_ENTITY_PLAYER,
  MRG_ENTITY_CURSOR,
  MRG_ENTITY_BAT,
  // a spwaner is a placeholder entity intended for
  // the map generator
  MRG_ENTITY_ENEMY_SPAWNER,
  MRG_ENTITY_LOOT_SPWANER,
  MRG_ENTITY_PROP_SPAWNER
};

enum mrg_entity_flags { MRG_ENTITY_FLAG_ALLOCED = 1 };

enum mrg_entity_stats {
  MRG_STAT_LEVEL = 0,
  MRG_STAT_HP = 1,
  MRG_STAT_HP_MAX = 2,

  // generic stats for per-entity config
  MRG_STAT_USTAT1 = 3,
  MRG_STAT_USTAT2 = 4,
  MRG_STAT_VEL_MAX,
  MRG_STAT_ACCEL,
  MRG_STATS_LEN
};

enum mrg_entity_behavior {
  MRG_BEH_NOP,
  MRG_BEH_PLAYER_UPDATE,
  MRG_BEH_ENTITY_DRAW,
  MRG_BEH_CURSOR_UPDATE,
  MRG_BEH_CURSOR_DRAW,
  MRG_BEH_ENTITY_DRAW_ALT,
};

// update functions that translate behaviors into commands
// that will eventually manipulate the scene
// commands are kept simple so that they may be serialized
// later on
typedef int (*mrg_entity_tick)(struct mrg_state *state,
                               struct mrg_entity *entity);

struct mrg_entity {
  int32_t room_id;
  // position
  mrg_fixed x;
  mrg_fixed y;
  int32_t flags;
  enum mrg_entities type;
  char tile_set[MRG_IDC_FILE_NAME_LEN];

  enum mrg_entity_behavior next_behavior;
  enum mrg_entity_behavior next_draw;

  int animation_timer;

  int vel_x;
  int vel_y;

  int col_offset_x;
  int col_offset_y;
  int col_w;
  int col_h;

  int tileset_id;
  int tile_id;

  // entity stats
  int stats[MRG_STATS_LEN];

  // entity specific flags
  int16_t uflags;
};

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MRG_SIGN(x) ((x) >= 0 ? 1 : (-1))

#define MRG_TILE_W 16
#define MRG_TILE_H 16

// uncomment to remove debug features
#define MRG_DEBUG
// uncomment to remove perftop outputs
#define MRG_PRFTOP

#define mrg_panic(ec, msg)                                                     \
  {                                                                            \
    fprintf(stderr, (msg));                                                    \
    exit(ec);                                                                  \
  }

#ifdef MRG_PRFTOP

#define MRG_PRFSTART() fprintf(stdout, "prftop-start %s\n", __func__);
#define MRG_PRFEND() fprintf(stdout, "prftop-end %s\n", __func__);

#else

#define MRG_PRGSTART()
#define MRG_PRFEND()

#endif

#endif
