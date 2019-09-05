#include "builtin.h"

// returns true if the 'exit' call
// should be performed
// (It must not be called here)
int exit_shell(char* cmd) {

	char *auxCmd = cmd;
	size_t i;

	for(i = 0; auxCmd[i] != SPACE && auxCmd[i] != END_STRING; i++) 
		auxCmd[i] = auxCmd[i + 1];	

	if(!strncmp(auxCmd, "exit", strlen("exit")))
		return 0;
	
	return -1;
}

// returns true if "chdir" was performed
//  this means that if 'cmd' contains:
// 	1. $ cd directory (change to 'directory')
// 	2. $ cd (change to HOME)
//  it has to be executed and then return true
//  Remember to update the 'prompt' with the
//  	new directory.
//
// Examples:
//  1. cmd = ['c','d', ' ', '/', 'b', 'i', 'n', '\0']
//  2. cmd = ['c','d', '\0']
int cd(char* cmd) {
	
	char *auxCmd = cmd;
	size_t i;

	for(i = 0; auxCmd[i] != SPACE && auxCmd[i] != END_STRING; i++) 
		auxCmd[i] = auxCmd[i + 1];

	if(strncmp(auxCmd, "cd", strlen("cd")))
		return -1;

	i = block_contains(auxCmd, '/'); //Es condicion?

	if(i > 0) { //No hay error
		if(chdir(auxCmd + i + 1))
			return -1;
	}

	else 
		chdir(getenv("HOME"));
	
	strcpy(promt, getenv("HOME"));

	return 0;
}

// returns true if 'pwd' was invoked
// in the command line
// (It has to be executed here and then
// 	return true)
int pwd(char* cmd) {

	char* buffer;
	size_t i, bufferSize;

	buffer = (char *) malloc(sizeof(char) * INIT_CHOP);

	if(!buffer)
		return -1;

	bufferSize = INIT_CHOP * sizeof(char);

	for(i = 0; cmd[i] != SPACE && cmd[i] != END_STRING; i++) 
		cmd[i] = cmd[i + 1];

	if(!strncmp(cmd, "pwd", strlen("pwd"))) {
		while(!getcwd(buffer, bufferSize))
			buffer = realloc(buffer, bufferSize *= INC_CHOP);
		
		fprintf(stdout, "%s\n", buffer);
		return 0;
	}
	
	return -1;
}

