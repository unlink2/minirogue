#ifndef IDC_H_
#define IDC_H_

#include <stddef.h>
#include <stdint.h>
#include "defs.h"

/**
 * Data fromat holding all game idca
 * all numbers are in little endian
 * File extension: .idc (integrated idca container)
 * Notes:
 *  - Tile set handles: tile sets are stored as \0 terminated strings for
 *    headers that required a tile set. They are then loaded and the apropriate
 *    handle is stored in the parsed result.
 *    Tile sets are looked for in the assets folder.
 *    File names may not be longer than MRG_IDC_FILE_NAME_LEN
 */

// magic identifier
#define MRG_IDC_MAGIC "idc"
#define MRG_IDC_FILE_NAME_LEN 8

#define MRG_IDC_LE(n) (n)

/**
 * File header:
 * id: always MRG_IDC_MAGIC
 * n_entries: amount of entries in the directory
 * directory_offset: where the directory is located in the file
 */
struct mrg_idc_header {
  const char magic[3];
  int32_t n_entries;
  int32_t directory_offset;
  int32_t chksm;
};

enum mrg_idc_dir_type { MRG_IDC_DIR_ROOM, MRG_IDC_DIR_ENTITY };

/**
 * Directory entry
 * type: directory entry typr
 * offset: location of idca in file
 * len: length of idca in file
 */
struct mrg_idc_dir {
  int32_t type;
  int32_t offset;
  int32_t len;
  void *entry; // actual data of this entry
};

/**
 * Entity entry
 * x: x position
 * y: y position
 * flags: entity flags
 * type: entity type
 * tile_set_handle: see note on tile set loading
 */
struct mrg_idc_entity {
  int32_t room_id;
  int32_t x;
  int32_t y;
  int32_t flags;
  int32_t type;
  char tile_set[MRG_IDC_FILE_NAME_LEN];
};

/**
 * Room definition
 * n_entities: amount of entities to load
 * entities_offset: location of entity list in file
 * room_w: room width
 * room_h: room height
 * tile_map_offset: location of tile map for the room (load room_w * room_h)
 * bytes) flags_offset: location of flag map for the room (load room_w * room_h
 * bytes)
 * tile_set_handle: see note on tile set loading
 */
struct mrg_idc_room {
  int32_t room_id;
  int32_t room_w;
  int32_t room_h;
  int32_t tile_map_offset;
  int32_t flags_offset;
  char tile_set[MRG_IDC_FILE_NAME_LEN];
};

struct mrg_idc_file {
  struct mrg_idc_header header;
  struct mrg_idc_dir *dirs;
};

struct mrg_idc_file mrg_idc_de(struct mrg_arena *a, const char *data,
                               size_t len);

const char *mrg_idc_se(struct mrg_arena *a, struct mrg_idc_file *f);

#endif
