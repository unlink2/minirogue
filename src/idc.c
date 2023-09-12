#include "idc.h"
#include "arena.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define MRG_IDC_READ(dst, src, len, current)                                   \
  memcpy((dst), (src) + (current), (len));                                     \
  (current) += (len);

#define MRG_IDC_READ_INT32(dst, src, len, current)                             \
  MRG_IDC_READ(dst, src, len, current);                                        \
  *dst = (int32_t)ntohl(*dst);

int32_t mrg_idc_chksm(const char *data, size_t len) {
  int32_t sum = 0;

  for (size_t i = 0; i < len; i++) {
    sum += data[i];
  }

  return sum;
}

struct mrg_idc_file mrg_idc_de(struct mrg_arena *a, const char *data,
                               size_t len) {
  struct mrg_idc_file file;
  memset(&file, 0, sizeof(file));

  int32_t chksm = mrg_idc_chksm(data, len);

  // parse header
  {
    size_t current = 0;
    struct mrg_idc_header header;
    size_t hlen = MRG_IDC_HEADER_LEN;
    if (len - current < hlen) {
      file.ok = -1;
      fprintf(stderr,
              "Unable to parse idc. Incomplete header (%ld bytes expected, got "
              "%ld bytes)\n",
              hlen, len);
      return file;
    }

    MRG_IDC_READ(header.magic, data, 3, current);
    MRG_IDC_READ(&header.version, data, 1, current);

    MRG_IDC_READ_INT32(&header.n_entries, data, sizeof(int32_t), current);
    MRG_IDC_READ_INT32(&header.directory_offset, data, sizeof(int32_t),
                       current);
    MRG_IDC_READ_INT32(&header.chksm, data, sizeof(int32_t), current);

    if (strncmp(MRG_IDC_MAGIC, header.magic, strlen(MRG_IDC_MAGIC)) != 0) {
      file.ok = -1;
      fprintf(stderr, "The provided file is not an idc file %c%c%c\n",
              header.magic[0], header.magic[1], header.magic[2]);
      return file;
    }

    // chksum is skipped if all 0!
    if (chksm != header.chksm && header.chksm != 0) {
      file.ok = -1;
      fprintf(stderr, "Invalid idc chekcsum: expected %x got %x!\n",
              header.chksm, chksm);
    }

    if (header.chksm == 0) {
      fprintf(stdout, "Warning: idc checksum is 0... skipping!\n");
    }

    file.header = header;
  }

  // directory
  {
    size_t current = file.header.directory_offset;
    file.dirs =
        mrg_arena_malloc(a, file.header.n_entries * sizeof(struct mrg_idc_dir));

    if (!file.dirs) {
      fprintf(stdout, "Failed to allocated idc directory! oom!\n");
      file.ok = -1;
      return file;
    }

    for (size_t i = 0; i < file.header.n_entries; i++) {
      size_t dirlen = MRG_IDC_DIR_LEN;
      if (len - current < dirlen) {
        file.ok = -1;
        fprintf(stderr, "Unable to parse idc. Incomplete directory!\n");
        return file;
      }

      struct mrg_idc_dir dir = {0, 0, NULL};

      MRG_IDC_READ_INT32(&dir.type, data, sizeof(int32_t), current);
      MRG_IDC_READ_INT32(&dir.offset, data, sizeof(int32_t), current);

      file.dirs[i] = dir;
    }
  }

  // entries
  {
    for (size_t i = 0; i < file.header.n_entries; i++) {
      struct mrg_idc_dir *dir = &file.dirs[i];
      size_t current = dir->offset;
      size_t entrylen = MRG_IDC_ENTRY_LEN;
      if (len - current < entrylen) {
        file.ok = -1;
        fprintf(stderr, "Unable to parse idc. Incomplete entry!\n");
        return file;
      }

      dir->entry = mrg_arena_malloc(a, sizeof(struct mrg_idc_entry));
      if (!dir->entry) {
        fprintf(stdout, "Failed to allocated idc entry! oom!\n");
        file.ok = -1;
        return file;
      }

      struct mrg_idc_entry *entry = dir->entry;
      memset(entry, 0, sizeof(struct mrg_idc_entry));

      switch (dir->type) {
      case MRG_IDC_DIR_ENTITY:
        MRG_IDC_READ_INT32(&entry->entity.room_id, data, sizeof(int32_t),
                           current);
        MRG_IDC_READ_INT32(&entry->entity.x, data, sizeof(int32_t), current);
        MRG_IDC_READ_INT32(&entry->entity.y, data, sizeof(int32_t), current);
        MRG_IDC_READ_INT32(&entry->entity.flags, data, sizeof(int32_t),
                           current);
        MRG_IDC_READ_INT32(&entry->entity.type, data, sizeof(int32_t), current);
        MRG_IDC_READ(&entry->entity.tile_set, data, MRG_IDC_FILE_NAME_LEN,
                     current);
        break;
      case MRG_IDC_DIR_ROOM:
        MRG_IDC_READ_INT32(&entry->room.room_id, data, sizeof(int32_t),
                           current);
        MRG_IDC_READ_INT32(&entry->room.room_w, data, sizeof(int32_t), current);
        MRG_IDC_READ_INT32(&entry->room.room_h, data, sizeof(int32_t), current);
        MRG_IDC_READ_INT32(&entry->room.tiles_offset, data, sizeof(int32_t),
                           current);
        MRG_IDC_READ_INT32(&entry->room.flags_offset, data, sizeof(int32_t),
                           current);

        MRG_IDC_READ(&entry->room.tile_set, data, MRG_IDC_FILE_NAME_LEN,
                     current);

        int32_t tiles_len = entry->room.room_w * entry->room.room_h;
        entry->room.tiles = mrg_arena_malloc(a, tiles_len);
        entry->room.flags = mrg_arena_malloc(a, tiles_len);
        if (!entry->room.tiles || !entry->room.flags) {
          fprintf(stdout, "Failed to allocated idc room maps! oom!\n");
          file.ok = -1;
          return file;
        }

        current = entry->room.tiles_offset;
        if (len - current < tiles_len) {
          file.ok = -1;
          fprintf(stderr, "Unable to parse idc. Incomplete tile map!\n");
          return file;
        }
        MRG_IDC_READ(entry->room.tiles, data, tiles_len, current);

        current = entry->room.flags_offset;
        if (len - current < tiles_len) {
          file.ok = -1;
          fprintf(stderr, "Unable to parse idc. Incomplete flag map!\n");
          return file;
        }
        MRG_IDC_READ(entry->room.flags, data, tiles_len, current);
        break;
      }
    }
  }

  return file;
}

const char *mrg_idc_se(struct mrg_arena *a, struct mrg_idc_file *f) {
  return NULL;
}
