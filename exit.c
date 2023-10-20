#include "shell.h"

/**
* exit_arg - handles the exit command
* @args: tokenized command
* @line: input read from stdin
*
* Return: no return
*/
void exit_arg(char **args, char *line)
{
	free(line);
	free_buffer(args);
	exit(0);
}
