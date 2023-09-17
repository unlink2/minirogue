#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mrg.h"
#include "argtable2.h"

struct arg_lit *verb = NULL;
struct arg_lit *help = NULL;
struct arg_lit *version = NULL;

// execute a command at startup
struct arg_str *exec = NULL;

// arg end stores errors
struct arg_end *end = NULL;

#define mrg_argtable                                                           \
  { help, version, verb, exec, end, }

void mrg_args_free(void) {
  void *argtable[] = mrg_argtable;
  arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void mrg_args_parse(int argc, char **argv) {
  help = arg_litn(NULL, "help", 0, 1, "display this help and exit");
  version = arg_litn(NULL, "version", 0, 1, "display version info and exit");
  verb = arg_litn("v", "verbose", 0, 1, "verbose output");
  exec = arg_strn("e", "exec", "Engine command", 0, 256, "Execute a command");
  end = arg_end(20);

  void *argtable[] = mrg_argtable;

  // output params
  char progname[] = "mrg";
  char short_desc[] = "";

  // version info
  int version_major = 0;
  int version_minor = 0;
  int version_patch = 1;

  int nerrors = arg_parse(argc, argv, argtable);
  int exitcode = 0;

  if (help->count > 0) {
    printf("Usage: %s", progname);
    arg_print_syntax(stdout, argtable, "\n");
    printf("%s\n\n", short_desc);
    arg_print_glossary(stdout, argtable, "  %-25s %s\n");
    goto exit;
  }

  if (version->count) {
    printf("%s version %d.%d.%d\n", progname, version_major, version_minor,
           version_patch);
    goto exit;
  }

  if (nerrors > 0) {
    arg_print_errors(stdout, end, progname);
    printf("Try '%s --help' for more information.\n", progname);
    exitcode = 1;
    goto exit;
  }
  return;
exit:
  mrg_args_free();
  exit(exitcode); // NOLINT
}

int main(int argc, char **argv) {
  mrg_args_parse(argc, argv);

  // map args to cfg here
  struct mrg_config cfg;
  memset(&cfg, 0, sizeof(cfg));

  cfg.exec = exec->sval;
  cfg.exec_len = exec->count;

  cfg.verbose = verb->count > 0;
  int res = mrg_main(&cfg);

  mrg_args_free();
  return res;
}
