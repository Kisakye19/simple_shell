#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "shell.h"

void execute_command(char *input)
{
    char *token;
    int i = 0;
    char **command;
    pid_t child_pid;
    int status;

    token = strtok(input, " \t\n");
    command = malloc(sizeof(char *) * 1024);

    while (token)
    {
        command[i] = token;
        token = strtok(NULL, " \t\n");
        i++;
    }

    command[i] = NULL;
    child_pid = fork();

    if (child_pid == 0)
    {
        if (execve(command[0], command, NULL) == -1)
            perror("Error:");
    }
    else
    {
        wait(&status);
    }

    free(command);
}
