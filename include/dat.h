#ifndef DAT_H_
#define DAT_H_

#include <stdint.h>

/**
 * Data fromat holding all game data
 * all numbers are in little endian
 */

// magic identifier
#define MRG_DAT_ID "mrgd"

#define MRG_DAT_LE(n) (n)

/**
 * File header:
 * id: always 'mrgd'
 * n_entries: amount of entries in the directory
 * directory_offset: where the directory is located in the file
 */
struct mrg_dat_header {
  const char id[4];
  int32_t n_entries;
  int32_t directory_offset;
};

/**
 * Directory entry
 * name: 8 byte identifier for file name
 * offset: location of data in file
 * len: length of data in file
 */
struct mrg_dat_dir {
  const char name[8];
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
struct mrg_dat_room {
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
struct mrg_dat_entity {
  int32_t x;
  int32_t y;
  int32_t flags;
  int32_t type;
};

#endif
