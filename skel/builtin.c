#include "builtin.h"

//Parte 2.1

// returns true if the 'exit' call
// should be performed
// (It must not be called here)
int exit_shell(char* cmd) {

	char *auxCmd = cmd;
	size_t i;

	for(i = 0; auxCmd[i] == SPACE; i++) 
		auxCmd[i] = auxCmd[i + 1];	

	if(strncmp(auxCmd, "exit", strlen("exit")))
		return 0;

	/*if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = -WTERMSIG(status);
	else if (WTERMSIG(status))
		status = -WSTOPSIG(status);*/
	
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
	int i = 0, j = 0;

	strcpy(auxCmd, cmd);

	for(i = 0; auxCmd[i] == SPACE; i++) 
		auxCmd[i] = auxCmd[i + 1];

	if(strncmp(auxCmd, "cd", strlen("cd")))
		return 0;

	//Caso cd, falta ver caso borde .. en home
	i = block_contains(auxCmd, '.');

	if(i > 0 && auxCmd[i + 1] == '.' && auxCmd[i + 2] == '\0') {
		i = block_contains(promt, ')');
	
		j = i;
		while(promt[j] != '/')
			j--;

		promt[j] = '\0';

		//printf("%s", promt + 1);
		if(!chdir(promt + 1)) {
			promt[j] = ')';
			promt[j + 1] = '\0';
			return 1;
		}
	}

	i = block_contains(auxCmd, '/');

	//Hay que considerar mas casos y bugs
	if(i > 0) { //No hay error
		if(!chdir(auxCmd + i + 1)) { //Validar
			promt[strlen(promt) - 1] = '/';
			strcat(promt, auxCmd + i + 1);
			promt[strlen(promt)] = ')'; //Aca preguntar por espacio promt
			promt[strlen(promt)] = '\0';
			return 1;
		}
	}
	else { 
		i = strlen("cd");
		
		if(auxCmd[i] != '\0') {

			while(auxCmd[i] == SPACE)
				i++;

			if(!chdir(auxCmd + i)) { //Validar
				promt[strlen(promt) - 1] = '/';
				strcat(promt, auxCmd + i);
				promt[strlen(promt)] = ')'; //Aca preguntar por espacio promt
				promt[strlen(promt)] = '\0';
			}
		}
		else{
			chdir(getenv("HOME"));
			strcpy(promt, getenv("HOME"));
		}
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

	char* buffer;
	size_t i, bufferSize;

	for(i = 0; cmd[i] == SPACE; i++) 
		cmd[i] = cmd[i + 1];

	if(strncmp(cmd, "pwd", strlen("pwd")))
		return 0;

	buffer = (char *) malloc(sizeof(char) * INIT_CHOP);

	if(!buffer)
		return -1; //Habra que validarlo o usar un buffer fijo

	bufferSize = INIT_CHOP * sizeof(char);

	while(!getcwd(buffer, bufferSize)){
		buffer = realloc(buffer, bufferSize *= INC_CHOP);
		if(!buffer)
			return -1;
	}
		
	fprintf(stdout, "%s\n", buffer);

	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = -WTERMSIG(status);
	else if (WTERMSIG(status))
		status = -WSTOPSIG(status);

	return 1;
}

/*int command_?(char* cmd)
{
	//Igual a parte 2.2
}*/


/*Pendiente:

	- Variables de entorno 2.2
	-magica
	-Lo ultimo

*/