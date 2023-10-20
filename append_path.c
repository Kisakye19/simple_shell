#include "shell.h"

/**
* append_path - adds path to command
* @path: path of command
* @command: user entered command
*
* Return: buffer containing command with path on success
* NULL on failure
*/
char *custom_append_paths(char *path, char *args)
{
	char *buf;
	size_t i = 0, j = 0;

	if (args == 0)
		args = "";

	if (path == 0)
		path = "";

	buf = malloc(sizeof(char) * (_strlen(path) + _strlen(args) + 2));
	if (!buf)
		return (NULL);

	while (path[i])
	{
		buf[i] = path[i];
		i++;
	}

	if (path[i - 1] != '/')
	{
		buf[i] = '/';
		i++;
	}
	while (args[j])
	{
		buf[i + j] = args[j];
		j++;
	}
	buf[i + j] = '\0';
	return (buf);
}
