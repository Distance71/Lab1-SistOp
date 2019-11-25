#ifndef EXEC_H
#define EXEC_H

#include "defs.h"
#include "types.h"
#include "utils.h"
#include "freecmd.h"

extern struct cmd* parsed_pipe;

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define STDOUTAPPEND 3

void exec_cmd(struct cmd* c);

#endif // EXEC_H
