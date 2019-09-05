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
static int open_redir_fd(char* file, int flags) {

	/*int fileNameIn, fileNameOut, fileNameErr;

	if(flags < 0 || flags > 2)
		return -1;

	if(flags == 0) {
		if((fileNameIn = open(file, O_RDONLY)) < 0)
				return -1;
		if((dup2(fileNameIn, flags)) < 0)
			return -1;

		close(fileNameIn);
	}
	if(flags == 1) {
		if((fileNameOut = open(file, O_CREAT, S_IWUSR || S_IRUSR)) < 0)
				return -1;
		if((dup2(fileNameOut, flags)) < 0)
			return -1;
	
		close(fileNameOut);
	}

	if(flags == 2) {
		if((fileNameErr = open(file, O_CREAT, S_IWUSR || S_IRUSR)) < 0)
				return -1;
		if((dup2(fileNameErr, flags)) < 0)
			return -1;
	
		close(fileNameErr);
	}*/

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

	struct execcmd* auxLeft;
	struct execcmd* auxRight;	

	switch (cmd->type) {

		case EXEC:
			// spawns a command
			//

			e = (struct execcmd*) cmd;
			execvp((e->argv)[0], e->argv);
			
			break;

		case BACK: {
			// runs a command in background
			//
			// Your code here
			printf("Background process are not yet implemented\n");
			_exit(-1);
			break;
		}

		case REDIR: {
			// changes the input/output/stderr flow
			//
			// To check if a redirection has to be performed
			// verify if file name's length (in the execcmd struct)
			// is greater than zero
			
			/*r = (struct execcmd*) cmd;

			if(strlen(r->out_file))
				open_redir_fd(r->out_file, 0);

			if(strlen(r->in_file))
				open_redir_fd(r->in_file, 1);

			if(strlen(r->err_file))
				open_redir_fd(r->err_file, 2);*/

			//execvp((e->argv)[0], e->argv);
			break;
		}
		
		case PIPE: {
			// pipes two commands
			//
			/*int pipefd[2];

			pipe(pipefd);

			dup2(pipefd[0], 0);
			dup2(pipefd[1], 1);

			close(pipefd[0]);
			close(pipefd[1]);

			p = (struct pipecmd*) cmd;
			auxLeft = (struct execcmd*) p->leftcmd;
			auxRight = (struct execcmd*) p->rightcmd;
			
			execvp((auxLeft->argv)[0], auxLeft->argv); //Uso Fork?
			execvp((auxRight->argv)[0], auxRight->argv);*/			

			// free the memory allocated
			// for the pipe tree structure
			free_command(parsed_pipe);

			break;
		}
	}
}

