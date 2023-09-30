#include "command.h"
#include "defs.h"
#include "entity.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "idc.h"
#include "maped.h"
#include "mrg.h"

int mrg_cmd_help(void *fp, mrg_fputs puts, const struct mrg_cmd *cmd,
                 const char *args, const struct mrg_cmd *tbl,
                 struct mrg_state *state) {
  const size_t buffer_len = 512;
  char buffer[buffer_len];
  while (tbl->name) {
    memset(buffer, 0, buffer_len);
    strncat(buffer, tbl->name, buffer_len);
    strncat(buffer, " ", buffer_len);

    struct mrg_arg *args = (struct mrg_arg *)tbl->args;
    while (args && args->name) {
      if (args->optional) {
        strncat(buffer, "[", buffer_len);
      }
      strncat(buffer, args->name, buffer_len);
      if (args->optional) {
        strncat(buffer, "]", buffer_len);
      }
      strncat(buffer, " ", buffer_len);
      args++;
    }
    strncat(buffer, ": ", buffer_len);
    strncat(buffer, tbl->desc, buffer_len);

    puts(buffer, fp);

    tbl++;
  }

  return 0;
}

int mrg_cmd_idc_write(void *fp, mrg_fputs puts, const struct mrg_cmd *cmd,
                      const char *args, const struct mrg_cmd *tbl,
                      struct mrg_state *state) {
  size_t read = 0;
  if (mrg_arg_parse(state->console.prev_idc_path, sizeof(char *), &cmd->args[0],
                    args, &read) == -1 &&
      state->console.prev_idc_path[0] == '\0') {
    puts("Missing argument!\n", fp);
    return -1;
  }
  puts(state->console.prev_idc_path, fp);

  mrg_arena_clear(&state->tmp_arena);
  return mrg_idc_save(&state->tmp_arena, &state->idc,
                      state->console.prev_idc_path);
}

int mrg_cmd_idc_read(void *fp, mrg_fputs puts, const struct mrg_cmd *cmd,
                     const char *args, const struct mrg_cmd *tbl,
                     struct mrg_state *state) {
  size_t read = 0;
  if (mrg_arg_parse(state->console.prev_idc_path, sizeof(char *), &cmd->args[0],
                    args, &read) == -1 &&
      state->console.prev_idc_path[0] == '\0') {
    puts("Missing argument!\n", fp);
    return -1;
  }
  puts(state->console.prev_idc_path, fp);

  mrg_arena_clear(&state->tmp_arena);
  int rc = mrg_idc_load(&state->tmp_arena, &state->idc,
                        state->console.prev_idc_path);

  if (rc == -1) {
    mrg_panic(-1, "Failed to parse idc file!\n");
  }

  mrg_state_invalidate(state);

  return rc;
}

int mrg_arg_int(int *out, const char *args, size_t *read) {
  char buffer[64];
  const char *tok = mrg_tok(buffer, args, 64, read);

  if (!tok) {
    return -1;
  }

  *out = (int)strtol(tok, NULL, 0);
  return 0;
}

int mrg_arg_string(const char *args, char *buffer, size_t buffer_len,
                   size_t *read) {
  if (mrg_tok(buffer, args, buffer_len, read)) {
    return 0;
  }
  return -1;
}

int mrg_arg_parse(void *out, size_t out_len, const struct mrg_arg *arg,
                  const char *args, size_t *read) {
  switch (arg->type) {
  case MRG_ARG_STRING:
    return mrg_arg_string(args, out, out_len, read);
  case MRG_ARG_INT:
    return mrg_arg_int(out, args, read);
  case MRG_ARG_LITERAL:
    while (*args && isspace(*args)) {
      args++;
    }
    memcpy(out, args, strlen(args));
    return 0;
  }

  return -1;
}

int mrg_cmd_exit(void *fp, mrg_fputs puts, const struct mrg_cmd *cmd,
                 const char *args, const struct mrg_cmd *tbl,
                 struct mrg_state *state) {
  int exit_code = 0;
  size_t read = 0;
  mrg_arg_parse(&exit_code, sizeof(exit_code), &cmd->args[0], args, &read);

  fprintf(stderr, "Exiting with code %d\n", exit_code);
  exit(exit_code);

  return 0;
}

int mrg_cmd_entity_goto(void *fp, mrg_fputs puts, const struct mrg_cmd *cmd,
                        const char *args, const struct mrg_cmd *tbl,
                        struct mrg_state *state) {
  int handle = 0;
  int x = 0;
  int y = 0;
  size_t read = 0;

  mrg_arg_parse(&handle, sizeof(handle), &cmd->args[0], args, &read);
  args += read;

  mrg_arg_parse(&x, sizeof(x), &cmd->args[1], args, &read);
  args += read;

  mrg_arg_parse(&y, sizeof(y), &cmd->args[2], args, &read);
  args += read;

  fprintf(stderr, "Moving entity %d to %d/%d\n", handle, x, y);

  if (handle >= state->entity_tbl.slots_len) {
    puts("Invalid entity handle", fp);
    return -1;
  }
  struct mrg_entity *e = &state->entity_tbl.slots[handle];
  if (!(e->flags & MRG_ENTITY_FLAG_ALLOCED)) {
    puts("Entity is not allocated!", fp);
    return -1;
  }

  e->x = MRG_FIXED(x, 0);
  e->y = MRG_FIXED(y, 0);

  return 0;
}

int mrg_cmd_set_stat(void *fp, mrg_fputs puts, const struct mrg_cmd *cmd,
                     const char *args, const struct mrg_cmd *tbl,
                     struct mrg_state *state) {
  int handle = 0;
  int slot = 0;
  int stat = 0;
  size_t read = 0;

  mrg_arg_parse(&handle, sizeof(handle), &cmd->args[0], args, &read);
  args += read;

  mrg_arg_parse(&slot, sizeof(slot), &cmd->args[1], args, &read);
  args += read;

  mrg_arg_parse(&stat, sizeof(stat), &cmd->args[2], args, &read);
  args += read;

  if (slot >= MRG_STATS_LEN) {
    puts("Stat slot is out of bounds!", fp);
  }
  if (handle >= state->entity_tbl.slots_len) {
    puts("Invalid entity handle", fp);
    return -1;
  }
  struct mrg_entity *e = &state->entity_tbl.slots[handle];
  if (!(e->flags & MRG_ENTITY_FLAG_ALLOCED)) {
    puts("Entity is not allocated!", fp);
    return -1;
  }

  e->stats[slot] = stat;
  printf("Set entity %d's stat slot %d to %d\n", handle, slot, stat);

  return 0;
}

int mrg_cmd_entity_pos(void *fp, mrg_fputs puts, const struct mrg_cmd *cmd,
                       const char *args, const struct mrg_cmd *tbl,
                       struct mrg_state *state) {
  int handle = 0;
  size_t read = 0;

  char buffer[64];
  memset(buffer, 0, 64);

  mrg_arg_parse(&handle, sizeof(handle), &cmd->args[0], args, &read);
  args += read;

  if (handle >= state->entity_tbl.slots_len) {
    puts("Invalid entity handle", fp);
    return -1;
  }
  struct mrg_entity *e = &state->entity_tbl.slots[handle];
  if (!(e->flags & MRG_ENTITY_FLAG_ALLOCED)) {
    puts("Entity is not allocated!", fp);
    return -1;
  }

  sprintf(buffer, "Entity %d is at %d.%d/%d.%d\n", handle,
          MRG_FIXED_WHOLE(e->x), MRG_FIXED_FRACT(e->x), MRG_FIXED_WHOLE(e->y),
          MRG_FIXED_FRACT(e->y));
  puts(buffer, fp);

  return 0;
}

int mrg_cmd_find_player(void *fp, mrg_fputs puts, const struct mrg_cmd *cmd,
                        const char *args, const struct mrg_cmd *tbl,
                        struct mrg_state *state) {
  char buffer[64];
  memset(buffer, 0, 64);

  for (size_t i = 0; i < state->entity_tbl.slots_len; i++) {
    struct mrg_entity *e = &state->entity_tbl.slots[i];
    if (e->flags & MRG_ENTITY_FLAG_ALLOCED && e->type == MRG_ENTITY_PLAYER) {
      sprintf(buffer, "Player handle %ld", i);
      puts(buffer, fp);
    }
  }

  return 0;
}

const char *mrg_tok(char *dst, const char *tok, size_t dst_len, size_t *read) {
  *read = 0;
  if (!tok) {
    return NULL;
  }

  const char *dst_start = dst;

  size_t written = 0;
  // trim leading spaces
  while (*tok && isspace(*tok)) {
    (*read)++;
    tok++;
  }

  // no token left...
  if (!tok[0]) {
    return NULL;
  }

  while (written < dst_len - 1 && *tok && !isspace(*tok)) {
    *dst = *tok;
    dst++;
    tok++;
    written++;
    (*read)++;
  }

  *dst = '\0';

  return dst_start;
}

int mrg_cmd_init_maped(void *fp, mrg_fputs puts, const struct mrg_cmd *cmd,
                       const char *args, const struct mrg_cmd *tbl,
                       struct mrg_state *state) {
  return mrg_transition(state, MRG_MODE_MAPED);
}

int mrg_cmd_tdbg(void *fp, mrg_fputs puts, const struct mrg_cmd *cmd,
                 const char *args, const struct mrg_cmd *tbl,
                 struct mrg_state *state) {
  mrg_toggle_dbg(state);
  return 0;
}

int mrg_cmd_maped_play(void *fp, mrg_fputs puts, const struct mrg_cmd *cmd,
                       const char *args, const struct mrg_cmd *tbl,
                       struct mrg_state *state) {
  mrg_entity_tbl_clear(&state->entity_tbl);

  int handle = mrg_entity_alloc(&state->entity_tbl);
  if (handle == -1) {
    return -1;
  }

  // TODO: make player loading more generic

  // set up player for testing
  struct mrg_entity *e = &state->entity_tbl.slots[handle];
  mrg_entity_init_type(e, MRG_ENTITY_PLAYER);

  e->tileset_id = mrg_tile_set_load(&state->tile_tbl, state->platform,
                                    "dbg.png", state->tile_w, state->tile_h);

  return mrg_transition(state, MRG_MODE_GAME);
}

int mrg_cmd_spawn_entity(void *fp, mrg_fputs puts, const struct mrg_cmd *cmd,
                         const char *args, const struct mrg_cmd *tbl,
                         struct mrg_state *state) {

  const char *types[] = {"player", NULL};

  int flags = 0;
  const size_t tlen = 64;
  char type_str[tlen];
  memset(type_str, 0, tlen);

  size_t read = 0;

  mrg_arg_parse(&type_str, tlen, &cmd->args[0], args, &read);
  args += read;

  mrg_arg_parse(&flags, sizeof(flags), &cmd->args[1], args, &read);
  args += read;

  enum mrg_entities type = -1;
  for (int i = 0; types[i]; i++) {
    if (strncmp(types[i], type_str, tlen) == 0) {
      type = i;
      break;
    }
  }
  if (type == -1) {
    puts("Unknown entity type!", fp);
    puts("Valid types: player,", fp);

    return -1;
  }

  return 0;
}

int mrg_cmd_exec(void *fp, mrg_fputs puts, const char *args,
                 const struct mrg_cmd *tbl, struct mrg_state *state) {
  const size_t buffer_len = 256;
  char buffer[buffer_len];

  char out_buffer[buffer_len];

  size_t tok_read = 0;
  const char *cmd_name = mrg_tok(buffer, args, buffer_len, &tok_read);

  if (!cmd_name) {
    return 0;
  }

  while (tbl->name) {
    if (strncmp(tbl->name, cmd_name, buffer_len) == 0) {
      const struct mrg_cmd *cmd = tbl;
      return cmd->exec(fp, puts, cmd, args + tok_read, tbl, state);
    }
    tbl++;
  }

  memset(out_buffer, 0, buffer_len);
  strncat(out_buffer, "Unknown command: ", buffer_len);
  strncat(out_buffer, cmd_name, buffer_len);

  puts(out_buffer, fp);
  return -1;
}

const struct mrg_cmd mrg_cmd_tbl[] = {
    {"help", "Display this help message", mrg_cmd_help, NULL},
    {"exit",
     "Exit the program",
     mrg_cmd_exit,
     {{"exit-code", true, MRG_ARG_INT}, {NULL}}},
    {"goto",
     "Move entity to position",
     mrg_cmd_entity_goto,
     {{"entity-handle", true, MRG_ARG_INT},
      {"x", true, MRG_ARG_INT},
      {"y", true, MRG_ARG_INT},
      {NULL}}},
    {"player", "Find payer entity", mrg_cmd_find_player, NULL},
    {"spawn",
     "Spawn entity at player position",
     mrg_cmd_spawn_entity,
     {{"type", true, MRG_ARG_STRING}, {"flags", true, MRG_ARG_INT}, NULL}},
    {"tdbg", "Toggle debug mode", mrg_cmd_tdbg, NULL},
    {"pos",
     "Entity position",
     mrg_cmd_entity_pos,
     {{"entity-handle", true, MRG_ARG_INT}, {NULL}}},
    {"maped", "Enter map editor", mrg_cmd_init_maped, {{NULL}}},
    {"play",
     "Play the currently selected maped map",
     mrg_cmd_maped_play,
     {{NULL}}},
    {"setstat",
     "Set stats",
     mrg_cmd_set_stat,
     {{"entity-handle", true, MRG_ARG_INT},
      {"stat-slot", true, MRG_ARG_INT},
      {"stat-value", true, MRG_ARG_INT},
      {NULL}}},
    {"idcwrite",
     "Write idc file to disk",
     mrg_cmd_idc_write,
     {{"path", true, MRG_ARG_LITERAL}, {NULL}}},
    {"idcread",
     "Read idc file from disk",
     mrg_cmd_idc_read,
     {{"path", true, MRG_ARG_LITERAL}, {NULL}}},
    {NULL}};
