#include "shell.h"
/**
* main - carries out the read, execute then print output loop
* @ac: argument count
* @av: argument vector
* @envp: environment vector
*
* Return: 0
*/

int main(int ac, char **av, char *envp[])
{
	char *line = NULL, *pathargs = NULL, *path = NULL;
	size_t bufsize = 0;
	ssize_t linesize = 0;
	char **args = NULL, **paths = NULL;
	(void)envp, (void)av;
	if (ac < 1)
		return (-1);
	signal(SIGINT, handle_signal);
	while (1)
	{
		free_buffer(args);
		free_buffer(paths);
		free(pathargs);
		prompt_use();
		linesize = custom_getline(&line, &bufsize);
		if (linesize < 0)
			break;
		info.ln_count++;
		if (line[linesize - 1] == '\n')
			line[linesize - 1] = '\0';
		args = tokenize(line);
		if (args == NULL || *args == NULL || **args == '\0')
			continue;
		if (custom_checker(args, line))
			continue;
		path = find_paths();
		paths = tokenize(path);
		pathargs = test_path(paths, args[0]);
		if (!pathargs)
			perror(av[0]);
		else
			execution(pathargs, args);
	}
	if (linesize < 0 && flags.interactive)
		write(STDERR_FILENO, "\n", 1);
	free(line);
	return (0);
}
