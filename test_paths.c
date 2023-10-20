#include "shell.h"

/**
* test_paths - checks whether path is valid
* @path: tokenized path
* @args: user entered command
*
* Return: path appended with command on success
* NULL on failure
*/
char *test_path(char **path, char *args)
{
	int i = 0;
	char *output;

	while (path[i])
	{
		output = custom_append_paths(path[i], args);
		if (access(output, F_OK | X_OK) == 0)
			return (output);
		free(output);
		i++;
	}
	return (NULL);
}
