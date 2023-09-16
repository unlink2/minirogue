#include "command.h"
#include <string.h>

int mrg_cmd_exec(void *fp, mrg_fputs puts, const char *args,
                 const struct mrg_cmd *tbl) {
  // find tok...
  // find command and pass the remainder of args into exec
  return -1;
}

int mrg_cmd_help(void *fp, mrg_fputs puts, struct mrg_cmd *cmd,
                 const char *args, const struct mrg_cmd *tbl) {
  const size_t buffer_len = 4096;
  char buffer[buffer_len];
  while (tbl->name) {
    memset(buffer, 0, buffer_len);
    strncat(buffer, tbl->name, buffer_len);
    strncat(buffer, " ", buffer_len);

    struct mrg_arg *args = tbl->args;
    while (args && args->name) {
      strncat(buffer, args->name, buffer_len);
      strncat(buffer, " ", buffer_len);
      args++;
    }

    puts(buffer, fp);

    tbl++;
  }

  return 0;
}

const struct mrg_cmd mrg_cmd_tbl[] = {
    {"help", "Display this help message", mrg_cmd_help, NULL}};
