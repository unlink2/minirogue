#ifndef COMMAND_H_
#define COMMAND_H_

#include "defs.h"
#include <stdbool.h>
#include <stddef.h>

typedef int (*mrg_fputs)(const char *s, void *fp);

typedef int (*mrg_cmd_handler)(void *fp, mrg_fputs puts, struct mrg_cmd *cmd,
                               const char *args, const struct mrg_cmd *tbl);

enum mrg_arg_type { MRG_ARG_STRING, MRG_ARG_INT, MRG_ARG_FLOAT };

struct mrg_arg {
  const char *name;
  bool optional;
  enum mrg_arg_type type;
  union {
    const char *default_str;
    int default_int;
    float default_float;
  };
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
  struct mrg_arg *args;
};

const char *mrg_tok(char *dst, const char *tok, size_t *len);

int mrg_cmd_exec(void *fp, mrg_fputs puts, const char *args,
                 const struct mrg_cmd *tbl);

/**
 * A command table
 * The last element of the cmd table should have a name of NULL to indicate the
 * end of the list
 */
extern const struct mrg_cmd mrg_cmd_tbl[];

#endif
