#include "shell.h"

/**
* handle_builtin - handles execution of builtin functions
* @args: tokenized commands
* @line: input read from stdin
*
* Return: 1 if executed, 0 if not
*/
int handle_built_in(char **args, char *line)
{
	struct builtin builtin = {"env", "exit"};

	if (_strcmp(*args, builtin.env) == 0)
	{
		print_env();
		return (1);
	}
	else if (_strcmp(*args, builtin.exit) == 0)
	{
		exit_arg(args, line);
		return (1);
	}
	return (0);
}
