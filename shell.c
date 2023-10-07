#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
	size_t buf_size = 0;
	char *buf = NULL;
	char *token;
	int status, i = 0;
	char **args;
	char *command;
	pid_t child_pid;
	ssize_t read_chars;

	while (1)
	{
		write(1, "#cisfun$ ", 9);
		read_chars = getline(&buf, &buf_size, stdin);
		token = strtok(buf, "\t\n");
		args = malloc(sizeof(char *) * 1024);

		if (read_chars == -1)
		{
			if (feof(stdin))
			{
			printf("\n");
			break;
			}
			else
			{
			perror("getline");
			continue;
			}
			}
			if (buf[read_chars - 1] == '\n')
			buf[read_chars - 1] = '\0';
			command = token

		while (token)
		{
			args[i] = token;
			token = strtok(NULL, "\t\n");
			i++;
		}
			args[i] = NULL;
			child_pid = fork();

		if (child_pid == 0)
		{
			if (execve(args, command, NULL) == -1)
				perror("Error");
		}
		else
		{
			wait(&status);
		}
		i = 0;
		free(args);
	}
	return (0);
}
