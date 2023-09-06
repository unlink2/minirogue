#ifndef COMMAND_H_
#define COMMAND_H_

#include "defs.h"
#include "fxp.h"
#include <stddef.h>
#include <stdint.h>

#define MRG_CMDS_MAX 1024

enum mrg_cmds { MRG_CMD_ENTITY_MOVE, MRG_CMD_ENTITY_VEL_APPLY };

struct mrg_cmd {
  enum mrg_cmds type;
  int handle;
  union {
    struct {
      mrg_fixed x;
      mrg_fixed y;
    };
  };
};

struct mrg_cmd_tbl {
  struct mrg_cmd cmds[MRG_CMDS_MAX];
  size_t cmd_len;
  size_t cmd_max_len;
};

struct mrg_cmd_tbl mrg_cmd_tbl_init(void);

void mrg_cmd_tbl_push(struct mrg_cmd_tbl *tbl, struct mrg_cmd cmd);

int mrg_cmd_exec(struct mrg_state *state, struct mrg_cmd *cmd);  

int mrg_cmd_tbl_exec(struct mrg_state *state, struct mrg_cmd_tbl *tbl);

#endif
