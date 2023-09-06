#include "command.h"
#include "mrg.h"
#include <stdio.h>
#include <string.h>

struct mrg_cmd_tbl mrg_cmd_tbl_init(void) {
  struct mrg_cmd_tbl tbl;

  memset(&tbl, 0, sizeof(tbl));
  tbl.cmd_max_len = MRG_CMDS_MAX;

  return tbl;
}

void mrg_cmd_tbl_push(struct mrg_cmd_tbl *tbl, struct mrg_cmd cmd) {
  tbl->cmds[tbl->cmd_len++] = cmd;
}

int mrg_cmd_exec(struct mrg_state *state, struct mrg_cmd *cmd) {
  switch (cmd->type) {
  case MRG_CMD_ENTITY_VEL_APPLY: {
    struct mrg_entity *entity = &state->entity_tbl.slots[cmd->handle];
    entity->x += cmd->x;
    entity->y += cmd->y;
  } break;
  case MRG_CMD_ENTITY_MOVE: {
    struct mrg_entity *entity = &state->entity_tbl.slots[cmd->handle];
    entity->x = cmd->x;
    entity->y = cmd->y;
  } break;
  }

  return 0;
}

int mrg_cmd_tbl_exec(struct mrg_state *state, struct mrg_cmd_tbl *tbl) {
  for (size_t i = 0; i < tbl->cmd_len; i++) {
    if (mrg_cmd_exec(state, &tbl->cmds[i]) == -1) {
      return -1;
    }
  }
  return 0;
}
