#ifndef COMMAND_H_
#define COMMAND_H_

#include "defs.h"
#include <stddef.h>

typedef const char *(*mrg_cmd_handler)(char *dst, size_t dst_len,
                                       struct mrg_cmd *cmd, const char *args);

struct mrg_cmd {
  const char *name;
  const char *desc;
  mrg_cmd_handler exec;
};

const char *mrg_tok(char *dst, const char *tok, size_t *len);

#endif
