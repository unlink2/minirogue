#include "defaults.h"
#include "entity.h"
#include "idc.h"
#include <stdio.h>
#include <string.h>

#define MRG_DEFAULT_ROOM_W 34
#define MRG_DEFAULT_ROOM_H 32

int8_t tiles[MRG_DEFAULT_ROOM_W * MRG_DEFAULT_ROOM_H];
int8_t flags[MRG_DEFAULT_ROOM_W * MRG_DEFAULT_ROOM_H];

struct mrg_idc_dir dirs[2] = {
    {MRG_IDC_DIR_ROOM,
     0,
     {.room = {0, MRG_DEFAULT_ROOM_W, MRG_DEFAULT_ROOM_H, 0, 0, "dbg.png",
               tiles, flags}}},
    {MRG_IDC_DIR_ENTITY,
     0,
     {.entity = {0, MRG_FIXED(64, 0), MRG_FIXED(64, 0), 0, MRG_ENTITY_PLAYER,
                 "dbg.png"}}}};

struct mrg_idc_file default_idc = {
    0, {MRG_IDC_MAGIC, MRG_IDC_DEFAULT_VERSION, 2, 0, 0}, (void *)dirs};

struct mrg_idc_file mrg_default_idc(void) {
  memset(tiles, 0, (size_t)MRG_DEFAULT_ROOM_W * MRG_DEFAULT_ROOM_H);
  memset(flags, 0, (size_t)MRG_DEFAULT_ROOM_W * MRG_DEFAULT_ROOM_H);

  return default_idc;
}
