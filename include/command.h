#ifndef COMMAND_H_
#define COMMAND_H_

#include "defs.h"
#include <stdbool.h>
#include <stddef.h>

typedef int (*mrg_fputs)(const char *s, void *fp);

typedef int (*mrg_cmd_handler)(void *fp, mrg_fputs puts,
                               const struct mrg_cmd *cmd, const char *args,
                               const struct mrg_cmd *tbl,
                               struct mrg_state *state);

enum mrg_arg_type { MRG_ARG_STRING, MRG_ARG_INT, MRG_ARG_FLOAT };

struct mrg_arg {
  const char *name;
  bool optional;
  enum mrg_arg_type type;
};

struct mrg_cmd {
  const char *name;
  const char *desc;
  mrg_cmd_handler exec;
  /**
   * An args table (may be NULL)
   * The last element should have a name of NULL to indicate the
   * end of the args list
   */
  struct mrg_arg args[8];
};

/**
 * Get the next token and insert it into dst
 * dst_len is the length of the buffer
 * dst_len also gets replaced by the amount of bytes
 * used from tok
 */
const char *mrg_tok(char *dst, const char *tok, size_t dst_len, size_t *read);

int mrg_cmd_exec(void *fp, mrg_fputs puts, const char *args,
                 const struct mrg_cmd *tbl, struct mrg_state *state);

int mrg_arg_int(int *out, const char *args, size_t *read);
int mrg_arg_float(float *out, const char *args, size_t *read);
int mrg_arg_string(const char *args, char *buffer, size_t buffer_len,
                   size_t *read);
int mrg_arg_parse(void *out, size_t out_len, const struct mrg_arg *arg,
                  const char *args, size_t *read);

/**
 * A command table
 * The last element of the cmd table should have a name of NULL to indicate the
 * end of the list
 */
extern const struct mrg_cmd mrg_cmd_tbl[];

#endif
