#ifndef IDC_H_
#define IDC_H_

#include <stdint.h>

/**
 * Data fromat holding all game idca
 * all numbers are in little endian
 * File extension: .idc (integrated idca container)
 */

// magic identifier
#define MRG_IDC_ID "idc"
#define MRG_IDC_PATH_MAX 256

#define MRG_IDC_LE(n) (n)

/**
 * File header:
 * id: always 'mrgd'
 * n_entries: amount of entries in the directory
 * directory_offset: where the directory is located in the file
 */
struct mrg_idc_header {
  const char id[3];
  int32_t n_entries;
  int32_t directory_offset;
};

enum mrg_idc_dir_type { MRG_IDC_DIR_ROOM, MRG_IDC_DIR_TILESET };

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
};

/**
 * Room definition
 * n_entities: amount of entities to load
 * entities_offset: location of entity list in file
 * room_w: room width
 * room_h: room height
 * tile_map_offset: location of tile map for the room (load room_w * room_h
 * bytes) flags_offset: location of flag map for the room (load room_w * room_h
 * bytes)
 */
struct mrg_idc_room {
  int32_t n_entities;
  int32_t entities_offset;
  int32_t room_w;
  int32_t room_h;
  int32_t tile_map_offset;
  int32_t flags_offset;
};

/**
 * Entity entry
 * x: x position
 * y: y position
 * flags: entity flags
 * type: entity type
 */
struct mrg_idc_entity {
  int32_t x;
  int32_t y;
  int32_t flags;
  int32_t type;
};

/**
 * Load a tile set from an external resource
 */
struct mrg_idc_tile_set {
  // path relative to asset path where the tile set image is stored
  char path[MRG_IDC_PATH_MAX];
};

#endif
