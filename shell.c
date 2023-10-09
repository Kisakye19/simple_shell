#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    char *line;
    char **args;
    int status;

    while (1)
    {
        printf("($) ");
        line = read_line();
        args = split_line(line);
        status = execute(args);

        free(line);
        free(args);

        if (status)
            perror("Error");
    }

    return (0);
}

char *read_line(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    nread = getline(&line, &len, stdin);
    if (nread == -1)
    {
        if (feof(stdin))
        {
            free(line);
            exit(EXIT_SUCCESS); /* Exit on Ctrl+D (EOF) */
        }
        else
        {
            perror("getline");
	    free(line);
            exit(EXIT_FAILURE);
        }
	return (line);
    }

    return line;
}

char **split_line(char *line)
{
    int bufsize = 1024; /* Initial buffer size */
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n\a");
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += 1024; /* Increase buffer size */
            tokens = realloc(tokens, bufsize * sizeof(char *));
	    if (!tokens)
            {
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[position] = NULL; /* Null-terminate the argument list */
    return tokens;
}

int execute(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        /* Child process */
        if (execve(args[0], args, NULL) == -1)
        {
            perror("execute");
	}
            exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("fork");
    }
    else
	{
 /* Parent process */
        wait(&status);
    }	

    return 0;
}
