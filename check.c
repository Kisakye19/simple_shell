#include "shell.h"
/**
 *checker- checks to see weather its a built in function
 *@:arg tokenized user input
 *@buf: line drived fromgetline function
 *Return: 1 if cmd excuted 0 if cmd is not executed
 */
int custom_checker(char **arg, char *buf)
{
	if (handle_built_in(arg, buf))
		return (1);
	else if (**arg == '/')
	{
		execution(arg[0], arg);
		return (1);
	}
	return (0);
}
