#include "command.h"

const struct mrg_cmd *mrg_cmd_tbl_init(void) {
  static struct mrg_cmd tbl[] = {{"help", "Display this help message"}};

  return tbl;
}
