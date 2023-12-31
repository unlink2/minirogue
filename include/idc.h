#ifndef IDC_H_
#define IDC_H_

#include <stddef.h>
#include <stdint.h>
#include "defs.h"

/**
 * Data fromat holding all game idca
 * all numbers are in little endian
 * File extension: .idc (integrated idca container)
 * Note: the order of directory entries is not guaranteed!
 * Notes:
 *  - Tile set handles: tile sets are stored as \0 terminated strings for
 *    headers that required a tile set. They are then loaded and the apropriate
 *    handle is stored in the parsed result.
 *    Tile sets are looked for in the assets folder.
 *    File names may not be longer than MRG_IDC_FILE_NAME_LEN
 * - Once an entry type is defined it can never be changed! Changing the layout
 * would be a breaking change so consider adding types carefully! If a change is
 * required regardless, the entry will have to be a new type that composes of
 * the old and new type!
 */

/**
 * IDC header
 * ========================
 * | size | type | name   |
 * | 3    | bytes| magic  |
 * | 1    | int8 | version|
 * | 4    | int32| n_entrs|
 * | 4    | int32| dirof  |
 * ========================
 */

/**
 * IDC directory
 * ========================
 * | size | type | name   |
 * | 4    | int32| type   |
 * | 4    | int32| entryof|
 * ========================
 */

/**
 * Entity Entry
 * ========================
 * | size | type | name   |
 * | 4    | int32| room_id|
 * | 4    | fixed| x      |
 * | 4    | fixed| y      |
 * | 4    | int32| flags  |
 * | 4    | int32| type   |
 * | 8    | str  | tileset|
 * ========================
 */

/**
 * Room Entry
 * ========================
 * | size | type | name   |
 * | 4    | int32| room_id|
 * | 4    | int32| room_w |
 * | 4    | int32| room_h |
 * | 4    | int32| tilesof|
 * | 4    | int32| flagsof|
 * | 8    | str  | tileset|
 * ========================
 * Tile map at tilesof: room_w * room_h bytes
 * Flags map at flagspf: room_w * room_h bytes
 */

// magic identifier
#define MRG_IDC_MAGIC "idc"

#define MRG_IDC_HEADER_LEN 4 + sizeof(int32_t) * 3
#define MRG_IDC_DIR_LEN sizeof(int32_t) * 2

// an entry cannot be larger than this by default!
#define MRG_IDC_ENTRY_ENTITY_ROOM_LEN                                          \
  sizeof(int32_t) * 5 + MRG_IDC_FILE_NAME_LEN

#define MRG_IDC_LE(n) (n)

#define mrg_idc_room mrg_room
#define mrg_idc_entity mrg_entity

/**
 * File header:
 * id: always MRG_IDC_MAGIC
 * n_entries: amount of entries in the directory
 * directory_offset: where the directory is located in the file
 */
struct mrg_idc_header {
  char magic[3];
  int8_t version;
  int32_t n_entries;
  int32_t directory_offset;
  int32_t chksm;
};

enum mrg_idc_dir_type { MRG_IDC_DIR_ROOM, MRG_IDC_DIR_ENTITY };

struct mrg_idc_entry {
  union {
    struct mrg_idc_entity entity;
    struct mrg_idc_room room;
  };
};

/**
 * Directory entry
 * type: directory entry typr
 * offset: location of idca in file
 */
struct mrg_idc_dir {
  enum mrg_idc_dir_type type;
  int32_t offset;
  struct mrg_idc_entry entry; // actual data of this entry
};

struct mrg_idc_file {
  int ok;
  struct mrg_idc_header header;
  struct mrg_idc_dir *dirs;

  size_t prealloced_len;
};

// create an empty idc struct
struct mrg_idc_file mrg_idc_init(void);

struct mrg_idc_dir mrg_idc_dir_init(enum mrg_idc_dir_type type,
                                    struct mrg_idc_entry entry);

struct mrg_idc_file mrg_idc_de(struct mrg_arena *a, const char *data,
                               size_t len);

const char *mrg_idc_se(struct mrg_arena *a, struct mrg_idc_file *f,
                       size_t *len);

// insert a new directory entry
int mrg_idc_insert(struct mrg_idc_file *f, struct mrg_idc_dir entry);

// remove an entry from the idc
int mrg_idc_remove(struct mrg_idc_file *f, struct mrg_idc_dir *entry);

// save id from state into path
int mrg_idc_save(struct mrg_arena *a, struct mrg_idc_file *f, const char *path);

// load idc from path into state
int mrg_idc_load(struct mrg_arena *a, struct mrg_idc_file *idc,
                 const char *path);

void mrg_idc_free(struct mrg_idc_file *f);

#endif
