#include "command.h"

int mrg_cmd_exec(void *fp, mrg_fputs puts, const char *args,
                 const struct mrg_cmd *tbl) {
  // find tok...
  // find command and pass the remainder of args into exec
  return -1;
}

int mrg_cmd_help(void *fp, mrg_fputs puts, struct mrg_cmd *cmd,
                 const char *args, const struct mrg_cmd *tbl) {}

const struct mrg_cmd mrg_cmd_tbl[] = {
    {"help", "Display this help message", mrg_cmd_help, NULL}};
