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

    if (getline(&line, &len, stdin) == -1)
    {
        if (feof(stdin))
        {
            free(line);
            exit(EXIT_SUCCESS); /* Exit on Ctrl+D (EOF) */
        }
        else
        {
            perror("read_line");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}

char **split_line(char *line)
{
    int bufsize = 64; /* Initial buffer size */
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
    {
        perror("split_line");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n\a");
    while (token)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += 64; /* Increase buffer size */
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                perror("split_line");
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
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else
	{
 /* Parent process */
        wait(&status);
    }	

    return 0;
}
