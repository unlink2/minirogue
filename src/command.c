#include "command.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mrg_cmd_help(void *fp, mrg_fputs puts, const struct mrg_cmd *cmd,
                 const char *args, const struct mrg_cmd *tbl, struct mrg_state *state) {
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

int mrg_arg_int(int *out, const char *args, size_t *read) {
  char buffer[64];
  const char *tok = mrg_tok(buffer, args, 64, read);

  if (!tok) {
    return -1;
  }

  *out = (int)strtol(tok, NULL, 0);
  return 0;
}

int mrg_arg_float(float *out, const char *args, size_t *read) {
  char buffer[64];
  const char *tok = mrg_tok(buffer, args, 64, read);

  if (!tok) {
    return -1;
  }

  *out = strtof(tok, NULL);
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
  case MRG_ARG_FLOAT:
    return mrg_arg_float(out, args, read);
  }

  return -1;
}

int mrg_cmd_exit(void *fp, mrg_fputs puts, const struct mrg_cmd *cmd,
                 const char *args, const struct mrg_cmd *tbl, struct mrg_state *state) {
  int exit_code = 0;
  size_t read = 0;
  mrg_arg_parse(&exit_code, sizeof(exit_code), &cmd->args[0], args, &read);

  fprintf(stderr, "Exiting with code %d\n", exit_code);
  exit(exit_code);

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
    {NULL}};
