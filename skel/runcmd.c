#include "runcmd.h"

int status = 0;
struct cmd* parsed_pipe;


void handler(int sig)
{
    printf("==> terminado: PID: %d\n", getpid());
}

// runs the command in 'cmd'
int run_cmd(char* cmd) {
	
	pid_t p;
	struct cmd *parsed;

	char stack[SIGSTKSZ];
    stack_t ss = {
        .ss_sp = stack,
        .ss_flags = 0,
        .ss_size = SIGSTKSZ,
    };

    struct sigaction sa = {
        .sa_handler = handler,
        .sa_flags = SA_ONSTACK,
    };
    
	// if the "enter" key is pressed
	// just print the promt again
	if (cmd[0] == END_STRING)
		return 0;

	//Falta modificar bultins al ejecutar $?
	// cd built-in call
	if (cd(cmd))
		return 0;

	// exit built-in call
	if (exit_shell(cmd))
		return EXIT_SHELL;

	// pwd buil-in call
	if (pwd(cmd))
		return 0;

	/*if(command_?(cmd))
		return 0;*/

	// parses the command line
	parsed = parse_line(cmd);
	
	// forks and run the command
	if ((p = fork()) == 0) { //El hijo no pasa de aqui
		
		// keep a reference
		// to the parsed pipe cmd
		// so it can be freed later
		if (parsed->type == PIPE)
			parsed_pipe = parsed;

		exec_cmd(parsed);
	}

	// background process special treatment
	// Hint:
	// - check if the process is 
	// 	going to be run in the 'back'
	// - print info about it with 
	// 	'print_back_info()'
	//

	// store the pid of the process
	parsed->pid = p;

	// waits for the process to finish
	
	if(parsed->type == BACK) {
		sigaltstack(&ss, 0);
		setpgid(p, 0);
	    //sigfillset(&sa.sa_mask);
		print_back_info(parsed);
		sigaction(SIGTERM, &sa, NULL);
	}
	else{
		waitpid(p, &status, 0); //No es necesario llamarlo en background
		print_status_info(parsed);
	}
	
	free_command(parsed);

	return 0;
}