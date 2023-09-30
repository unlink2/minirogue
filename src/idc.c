#include "idc.h"
#include "arena.h"
#include "mrg.h"
#include "platform.h"
#include "defaults.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

/**
 * TODO: validate length when writing and reading
 */

#define MRG_IDC_READ(dst, src, len, current)                                   \
  memcpy((dst), (src) + (current), (len));                                     \
  (current) += (len);

#define MRG_IDC_READ_INT32(dst, src, len, current)                             \
  MRG_IDC_READ(dst, src, len, current);                                        \
  *(dst) = (int32_t)ntohl(*(dst));

size_t mrg_idc_entry_len(enum mrg_idc_dir_type type) {
  switch (type) {
  case MRG_IDC_DIR_ENTITY:
  case MRG_IDC_DIR_ROOM:
    return MRG_IDC_ENTRY_ENTITY_ROOM_LEN;
  default:
    assert(0);
  }

  return -1;
}

int32_t mrg_idc_chksm(const char *data, size_t len) {
  int32_t sum = 0;

  for (size_t i = 0; i < len; i++) {
    sum += data[i];
  }

  return sum;
}

struct mrg_idc_file mrg_idc_init(void) {
  struct mrg_idc_file f;
  memset(&f, 0, sizeof(f));

  memcpy(f.header.magic, MRG_IDC_MAGIC, 3);
  f.header.version = 0;

  return f;
}

struct mrg_idc_dir mrg_idc_dir_init(enum mrg_idc_dir_type type,
                                    struct mrg_idc_entry entry) {
  struct mrg_idc_dir dir;

  dir.type = type;
  dir.entry = entry;
  dir.offset = 0;
  return dir;
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

    fprintf(stderr, "Reading idc directory at offset %ld\n", current);

    file.dirs = malloc(file.header.n_entries * sizeof(struct mrg_idc_dir));

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

      struct mrg_idc_dir dir = {0, 0};
      memset(&dir, 0, sizeof(dir));

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
      size_t entrylen = mrg_idc_entry_len(dir->type);

      size_t remaining = len - current;
      if (remaining < entrylen) {
        file.ok = -1;
        fprintf(stderr,
                "Unable to parse idc. Incomplete entry at offset %ld! Expected "
                "%ld bytes, "
                "but got %ld bytes\n",
                current, entrylen, remaining);
        return file;
      }

      fprintf(stdout, "Reading entry %ld at offset %ld\n", i, current);

      struct mrg_idc_entry *entry = &dir->entry;
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
        entry->room.tiles = malloc(tiles_len);
        entry->room.flags = malloc(tiles_len);
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

#define MRG_IDC_WRITE(a, predef_dst, src, len, total)                          \
  {                                                                            \
    void *dst = (predef_dst);                                                  \
    if (!dst) {                                                                \
      return NULL;                                                             \
    }                                                                          \
    memcpy(dst, (src), (len));                                                 \
  }

#define MRG_IDC_WRITE_INT32(a, dst, src, total)                                \
  {                                                                            \
    int32_t val = (int32_t)htonl((src));                                       \
    MRG_IDC_WRITE(a, dst, &val, sizeof(int32_t), total);                       \
  }

#define MRG_IDC_WRITE_INT8(a, dst, src, total)                                 \
  {                                                                            \
    int8_t val = (src);                                                        \
    MRG_IDC_WRITE(a, dst, &val, sizeof(int8_t), total);                        \
  }

const char *mrg_idc_se(struct mrg_arena *a, struct mrg_idc_file *f,
                       size_t *len) {
  const char *start = (const char *)a->data + a->aptr;
  *len = 0;
  // header
  {
    *len += MRG_IDC_HEADER_LEN;
    MRG_IDC_WRITE(a, mrg_arena_malloc(a, 3), MRG_IDC_MAGIC, 3, len);
    MRG_IDC_WRITE_INT8(a, mrg_arena_malloc(a, sizeof(int8_t)), 0, len);
    MRG_IDC_WRITE_INT32(a, mrg_arena_malloc(a, sizeof(int32_t)), 2, len);
    MRG_IDC_WRITE_INT32(a, mrg_arena_malloc(a, sizeof(int32_t)),
                        MRG_IDC_HEADER_LEN, len);
    MRG_IDC_WRITE_INT32(a, mrg_arena_malloc(a, sizeof(int32_t)), 0, len);
  }

  // directory pre-allocation
  size_t dirs_len = f->header.n_entries * MRG_IDC_DIR_LEN;
  *len += dirs_len;
  int32_t *dst_dirs = mrg_arena_malloc(a, dirs_len);

  // entries + filling the respective dir entry
  {
    for (size_t i = 0; i < f->header.n_entries; i++) {
      struct mrg_idc_dir *dir = &f->dirs[i];
      struct mrg_idc_entry *entry = &dir->entry;

      size_t entry_len = mrg_idc_entry_len(dir->type);

      int32_t *dst_entry = mrg_arena_malloc(a, entry_len);
      *len += entry_len;

      size_t offset = (char *)dst_entry - start;
      MRG_IDC_WRITE_INT32(a, dst_dirs++, dir->type, len);
      MRG_IDC_WRITE_INT32(a, dst_dirs++, offset, len);

      switch (dir->type) {
      case MRG_IDC_DIR_ROOM: {
        int tiles_len = entry->room.room_w * entry->room.room_h;
        MRG_IDC_WRITE_INT32(a, dst_entry++, entry->room.room_id, len);
        MRG_IDC_WRITE_INT32(a, dst_entry++, entry->room.room_w, len);
        MRG_IDC_WRITE_INT32(a, dst_entry++, entry->room.room_h, len);
        MRG_IDC_WRITE_INT32(a, dst_entry++, offset + entry_len, len);
        MRG_IDC_WRITE_INT32(a, dst_entry++, offset + entry_len + tiles_len,
                            len);
        MRG_IDC_WRITE(a, dst_entry, entry->room.tile_set, MRG_IDC_FILE_NAME_LEN,
                      len);

        char *dst_tiles = mrg_arena_malloc(a, tiles_len);
        MRG_IDC_WRITE(a, dst_tiles, entry->room.tiles, tiles_len, len);

        char *dst_flags = mrg_arena_malloc(a, tiles_len);
        MRG_IDC_WRITE(a, dst_flags, entry->room.flags, tiles_len, len);

        *len += (size_t)tiles_len * 2;
        break;
      }
      case MRG_IDC_DIR_ENTITY:
        MRG_IDC_WRITE_INT32(a, dst_entry++, entry->entity.room_id, len);
        MRG_IDC_WRITE_INT32(a, dst_entry++, entry->entity.x, len);
        MRG_IDC_WRITE_INT32(a, dst_entry++, entry->entity.y, len);
        MRG_IDC_WRITE_INT32(a, dst_entry++, entry->entity.flags, len);
        MRG_IDC_WRITE_INT32(a, dst_entry++, entry->entity.type, len);
        MRG_IDC_WRITE(a, dst_entry, entry->room.tile_set, MRG_IDC_FILE_NAME_LEN,
                      len);
        break;
      }
    }
  }

  // TODO: implement checksum properly!

  return start;
}

int mrg_idc_insert(struct mrg_idc_file *f, struct mrg_idc_dir entry) {
  f->header.n_entries++;
  size_t len = f->header.n_entries * sizeof(struct mrg_idc_dir);
  if (f->dirs) {
    void *new_dirs = realloc(f->dirs, len);

    if (!new_dirs) {
      fprintf(stderr, "Inserting new idc directory failed!\n");
      return -1;
    }

    f->dirs = new_dirs;
  } else {
    f->dirs = malloc(len);
  }
  f->dirs[f->header.n_entries - 1] = entry;

  return 0;
}

int mrg_idc_remove(struct mrg_idc_file *f, struct mrg_idc_dir *entry) {
  if (f->header.n_entries == 1 && f->dirs) {
    f->header.n_entries = 0;
    free(f->dirs);
    f->dirs = NULL;
    return 0;
  } else if (f->header.n_entries < 1 || !f->dirs) {
    return -1;
  }

  // find the entry
  // and move the last entry into its position
  // unless the removed entry is the last entry!
  for (size_t i = 0; i < f->header.n_entries; i++) {
    if (&f->dirs[i] == entry) {
      f->dirs[i] = f->dirs[f->header.n_entries - 1];
      break;
    }
  }

  f->header.n_entries--;

  return 0;
}

int mrg_idc_save(struct mrg_arena *a, struct mrg_idc_file *f,
                 const char *path) {
  size_t len = 0;
  const char *data = mrg_idc_se(a, f, &len);

  if (!data || !path || strlen(path) == 0) {
    fprintf(stderr, "Unabel to serialize idc %s\n", path);
    return -1;
  }

  fprintf(stdout, "Writing idc to '%s'\n", path);

  return (int)mrg_pl_fwrite(path, data, len);
}

int mrg_idc_load(struct mrg_arena *a, struct mrg_idc_file *idc,
                 const char *path) {
  size_t len = 0;
  mrg_idc_free(idc);

  if (!path || strlen(path) == 0) {
    fprintf(stderr, "Unabel to deserialize idc %s\n", path);
    return -1;
  }

  fprintf(stdout, "Reading idc from '%s'\n", path);

  const char *data = mrg_pl_fread(a, path, &len);
  fprintf(stdout, "'%s' read %ld bytes\n", path, len);
  if (!data) {
    fprintf(stderr, "Unabel to deserialize idc %s\n", path);
    return -1;
  }

  *idc = mrg_idc_de(a, data, len);

  return idc->ok;
}

void mrg_idc_free(struct mrg_idc_file *f) {
  // free previous idc if it is not the default one!
  if (f->header.version == MRG_IDC_DEFAULT_VERSION) {
    return;
  }

  for (size_t i = 0; i < f->header.n_entries; i++) {
    if (f->dirs[i].type == MRG_IDC_DIR_ROOM) {
      free(f->dirs[i].entry.room.tiles);
      free(f->dirs[i].entry.room.flags);
    }
  }
  free(f->dirs);
}
