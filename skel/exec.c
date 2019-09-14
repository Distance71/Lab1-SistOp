#include "exec.h"

// sets the "key" argument with the key part of
// the "arg" argument and null-terminates it
static void get_environ_key(char* arg, char* key) {

	int i;
	for (i = 0; arg[i] != '='; i++)
		key[i] = arg[i];

	key[i] = END_STRING;
}

// sets the "value" argument with the value part of
// the "arg" argument and null-terminates it
static void get_environ_value(char* arg, char* value, int idx) {

	int i, j;
	for (i = (idx + 1), j = 0; i < strlen(arg); i++, j++)
		value[j] = arg[i];

	value[j] = END_STRING;
}

// sets the environment variables received
// in the command line
//
// Hints:
// - use 'block_contains()' to
// 	get the index where the '=' is
// - 'get_environ_*()' can be useful here
//Parte 2.2, no funciona ok
static void set_environ_vars(char** eargv, int eargc) {

	char auxName[BUFLEN], auxVal[BUFLEN];

	for (int i = 0; i < eargc; ++i)
	{
		get_environ_key(eargv[i], auxName);
		get_environ_value(eargv[i], auxVal, strlen(auxName));
		setenv(auxName, auxVal, 1);
	}	
} 

// opens the file in which the stdin/stdout or
// stderr flow will be redirected, and returns
// the file descriptor
// 
// Find out what permissions it needs.
// Does it have to be closed after the execve(2) call?
//
// Hints:
// - if O_CREAT is used, add S_IWUSR and S_IRUSR
// 	to make it a readable normal file
// Parte 3.1
static int open_redir_fd(char* file, int flags) {

	int fileNameIn, fileNameOut, fileNameErr;

	if(flags < 0 || flags > 3)
		return -1;

	if(flags == 0) {
		
		if((fileNameOut = open(file, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) < 0)
				return -1;
		if((dup2(fileNameOut, fileno(stdout))) < 0)
			return -1;
	
		close(fileNameOut);
	}

	if(flags == 1) {
		if((fileNameIn = open(file, O_RDONLY)) < 0)
				return -1;
		if((dup2(fileNameIn, fileno(stdin))) < 0)
			return -1;

		close(fileNameIn);
	}

	if(flags == 2) {
		if((fileNameErr = open(file, O_CREAT, S_IWUSR || S_IRUSR)) < 0)
				return -1;
		if((dup2(fileNameErr, fileno(stderr))) < 0)
			return -1;
	
		close(fileNameErr);
	}

	if(flags == 3) {
		if((fileNameOut = open(file, O_CREAT | O_APPEND, S_IRUSR | S_IWUSR)) < 0)
				return -1;
		if((dup2(fileNameOut, fileno(stdout))) < 0)
			return -1;
	
		close(fileNameOut);	
	}

	return 0;
}

// executes a command - does not return
//
// Hint:
// - check how the 'cmd' structs are defined
// 	in types.h
// - casting could be a good option
void exec_cmd(struct cmd* cmd) {

	// To be used in the different cases
	struct execcmd* e;
	struct backcmd* b;
	struct execcmd* r;
	struct pipecmd* p;

	switch (cmd->type) {

		case EXEC:
			// spawns a command
			//Parte 1.1

			e = (struct execcmd*) cmd;

			set_environ_vars(e->eargv, e->eargc);
			execvp((e->argv)[0], e->argv);
			
			break;

		case BACK: {
			// runs a command in background
			// Parte 2.3
			
			b = (struct backcmd*) cmd;
			exec_cmd(b->c);

			break;
		}

		case REDIR: {
			// changes the input/output/stderr flow
			//
			// To check if a redirection has to be performed
			// verify if file name's length (in the execcmd struct)
			// is greater than zero
			//Parte 3.1
			int auxPos;

			r = (struct execcmd*) cmd;

			//Ver caso especial 2>&1
			if(strlen(r->out_file)) {
				auxPos = block_contains(r->scmd, '>');
				if((r->scmd)[auxPos + 1] == '>')
					open_redir_fd(r->out_file, 3);
				else	
					open_redir_fd(r->out_file, 0);
			}

			if(strlen(r->in_file))
				open_redir_fd(r->in_file, 1);

			if(strlen(r->err_file))
				open_redir_fd(r->err_file, 2);
			

			//set_environ_vars(r->eargv, r->eargc);			
			execvp((r->argv)[0], r->argv);
			
			break;
		}
		
		case PIPE: {
			// pipes two commands
			// Parte 3.2
			int pipefd[2];
			pid_t pid;

			struct cmd* auxLeft;
			struct cmd* auxRight;

			pipe(pipefd);

			p = (struct pipecmd*) cmd;
			auxLeft = p->leftcmd;
			auxRight = p->rightcmd;

			if((pid = fork()) == 0) {
				wait(NULL);
				dup2(pipefd[0], fileno(stdin));
				close(pipefd[0]);
				exec_cmd(auxRight);
				close(pipefd[1]);
			}
			else {
				dup2(pipefd[1], fileno(stdout));
				close(pipefd[1]);
				exec_cmd(auxLeft);
				close(pipefd[0]);				
			}
			
			// free the memory allocated
			// for the pipe tree structure
			free_command(parsed_pipe);

			break;
		}
	}
}

