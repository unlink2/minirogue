#include "defaults.h"
#include "entity.h"
#include "idc.h"
#include <string.h>

int8_t tiles[32 * 32];
int8_t flags[32 * 32];

struct mrg_idc_entry room = {
    .room = {0, 32, 32, 0, 0, "dbg.png", tiles, flags}};
struct mrg_idc_entry entity = {
    .entity = {0, 16, 16, 0, MRG_ENTITY_PLAYER, "dbg.png"}};

struct mrg_idc_dir room_dir = {MRG_IDC_DIR_ROOM, 0, &room};
struct mrg_idc_dir entity_dir = {MRG_IDC_DIR_ENTITY, 0, &entity};

struct mrg_idc_dir *dirs[2] = {&room_dir, &entity_dir};

const struct mrg_idc_file default_idc = {
    0, {MRG_IDC_MAGIC, 0, 2, 0, 0}, (struct mrg_idc_dir *)dirs};

struct mrg_idc_file mrg_default_idc(void) {
  memset(tiles, 0, (size_t)32 * 32);
  memset(flags, 0, (size_t)32 * 32);
  return default_idc;
}
