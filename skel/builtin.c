#include "builtin.h"

//Parte 2.1

// returns true if the 'exit' call
// should be performed
// (It must not be called here)
int exit_shell(char* cmd) {

	size_t i = 0;

	while(cmd[i] == SPACE)
		i++;

	if(strcmp(cmd + i, "exit"))
		return 0;
	
	return 1;
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
	
	char auxCmd[BUFLEN];
	size_t i = 0;

	strcpy(auxCmd, cmd);

	for(i = 0; auxCmd[i] == SPACE; i++) 
		auxCmd[i] = auxCmd[i + 1];

	if(strncmp(auxCmd, "cd", strlen("cd")))
		return 0;

	for(i = strlen("cd"); auxCmd[i] == SPACE; i++) 
		auxCmd[i] = auxCmd[i + 1];

	if(auxCmd[i] != '\0') {
		if(!chdir(auxCmd + i) && strcmp(auxCmd + i, "..")) {
			promt[strlen(promt) - 1] = '/';
			strcat(promt, auxCmd + i);
			promt[strlen(promt)] = ')';
			promt[strlen(promt)] = '\0';
			return 1;
		}
		else if(!strcmp(auxCmd + i, "..")) {
			i = strlen(promt);
			while(promt[i] != '/') {
				i--;
			}
			promt[i++] = ')';
			promt[i] = '\0';
		}
	}
	else {
		chdir(getenv("HOME"));
		strcpy(promt, getenv("HOME"));
		memmove(promt + 1, promt, strlen(promt) + 1);
		memcpy(promt,"(",1);
		promt[strlen(promt)] = ')'; //Aca preguntar por espacio promt
		promt[strlen(promt)] = '\0';
	}

	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = -WTERMSIG(status);
	else if (WTERMSIG(status))
		status = -WSTOPSIG(status);

	return 1;
}

// returns true if 'pwd' was invoked
// in the command line
// (It has to be executed here and then
// 	return true)
int pwd(char* cmd) {

	char buffer[PATH_MAX];
	size_t i = 0;

	while(cmd[i] == SPACE)
		i++;

	if(strcmp(cmd + i, "pwd"))
		return 0;

	if(!getcwd(buffer, PATH_MAX))
		fprintf(stderr, "%s\n", ERROR_TAMANIO_DIRECTORIO);
		
	fprintf(stdout, "%s\n", buffer);

	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = -WTERMSIG(status);
	else if (WTERMSIG(status))
		status = -WSTOPSIG(status);

	return 1;
}