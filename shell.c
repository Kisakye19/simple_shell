#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ; 

void handle_exit() {
    printf("Exiting the shell\n");
    exit(0); 
}

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
	if (args[0] != NULL) 
	{
            if (strcmp(args[0], "exit") == 0) 
	    {
                handle_exit(); /* Call the exit handler*/
            } 
		else 
	status = execute(args, environ);

        if (status)
            perror("Error");
	}
	free(line);
        free(args);
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

int execute(char **args, char **environ)
{
    pid_t pid;
    int status;

    if (args == NULL || args[0] == NULL)
        return (1);

    pid = fork();
    if (pid == 0)
    {
      if (access(args[0], X_OK) == 0) 
	{
        /* Child process */
        if (execve(args[0], args, NULL) == -1)
    {
            perror(args[0]);
            exit(EXIT_FAILURE);
    }
  } else {
            /* If the command doesn't contain a path, search in PATH */
            char *path = getenv("PATH");
            char *token, *full_path, *dirs, *saveptr;
            
            if (path == NULL) {
                perror("No PATH found in environment");
                exit(EXIT_FAILURE);
            }

            dirs = strdup(path);
            if (dirs == NULL) {
                perror("strdup");
                exit(EXIT_FAILURE);
            }

            token = strtok_r(dirs, ":", &saveptr);
	while (token != NULL) {
                full_path = malloc(strlen(token) + strlen(args[0]) + 2);
                if (full_path == NULL) {
                    perror("malloc");
                    exit(EXIT_FAILURE);
                }
                sprintf(full_path, "%s/%s", token, args[0]);

                if (access(full_path, X_OK) == 0) 
		{
                    if (execve(full_path, args, environ) == -1) 
		    {
                        perror(args[0]);
                        free(full_path);
                        exit(EXIT_FAILURE);
                    }
                }
                free(full_path);
                token = strtok_r(NULL, ":", &saveptr);
            }
            perror(args[0]);
	    free(dirs);
            exit(EXIT_FAILURE);
        }
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

    return (1);
}
