#include "defs.h"
#include "types.h"
#include "readline.h"
#include "runcmd.h"

char promt[PRMTLEN] = {0};

// runs a shell command
static void run_shell() {

	char* cmd;

	while ((cmd = read_line(promt)) != NULL)
		if (run_cmd(cmd) == EXIT_SHELL)
			return;
}

// initialize the shell
// with the "HOME" directory
static void init_shell() {

	char buf[BUFLEN] = {0};
	char* home = getenv("HOME");

	if (chdir(home) < 0) {
		snprintf(buf, sizeof buf, "cannot cd to %s ", home);
		perror(buf);
	} else {
		snprintf(promt, sizeof promt, "(%s)", home);
	}	
}

//Mi codigo punto 1

/*char *path = getenv("PATH");
char *tmpstr = malloc(strlen(path)+strlen(executable)+2);*/



int main(void) {

	init_shell();

	run_shell();

	return 0;
}

