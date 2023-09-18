#include "defaults.h"
#include "entity.h"
#include "idc.h"
#include <stdio.h>
#include <string.h>

int8_t tiles[32 * 32];
int8_t flags[32 * 32];

struct mrg_idc_dir dirs[2] = {
    {MRG_IDC_DIR_ROOM, 0, {.room = {0, 32, 32, 0, 0, "dbg.png", tiles, flags}}},
    {MRG_IDC_DIR_ENTITY,
     0,
     {.entity = {0, MRG_FIXED(64, 0), MRG_FIXED(64, 0), 0, MRG_ENTITY_PLAYER,
                 "dbg.png"}}}};

struct mrg_idc_file default_idc = {
    0, {MRG_IDC_MAGIC, 0, 2, 0, 0}, (void *)dirs};

struct mrg_idc_file mrg_default_idc(void) {
  memset(tiles, 0, (size_t)32 * 32);
  memset(flags, 0, (size_t)32 * 32);

  return default_idc;
}
