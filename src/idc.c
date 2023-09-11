#include "idc.h"
#include "arena.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define MRG_IDC_READ(dst, src, len, current)                                   \
  memcpy((dst), (src) + (current), (len));                                     \
  (current) += (len);

int32_t mrg_idc_chksm(const char *data, size_t len) {
  int32_t sum = 0;

  for (size_t i = 0; i < len; i++) {
    sum += data[i];
  }

  return sum;
}

struct mrg_idc_file mrg_idc_de(struct mrg_arena *a, const char *data,
                               size_t len) {
  size_t current = 0;
  struct mrg_idc_file file;
  memset(&file, 0, sizeof(file));

  int32_t chksm = mrg_idc_chksm(data, len);

  // parse header
  {
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

    MRG_IDC_READ(&header.n_entries, data, sizeof(int32_t), current);
    header.n_entries = (int32_t)ntohl(header.n_entries);

    MRG_IDC_READ(&header.directory_offset, data, sizeof(int32_t), current);
    header.directory_offset = (int32_t)ntohl(header.directory_offset);

    MRG_IDC_READ(&header.chksm, data, sizeof(int32_t), current);
    header.chksm = (int32_t)ntohl(header.chksm);

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
    file.dirs = mrg_arena_mallocr(a, file.header.n_entries *
                                         sizeof(struct mrg_idc_dir));

    for (size_t i = 0; i < file.header.n_entries; i++) {
    }
  }

  // entries
  {}

  return file;
}

const char *mrg_idc_se(struct mrg_arena *a, struct mrg_idc_file *f) {
  return NULL;
}
