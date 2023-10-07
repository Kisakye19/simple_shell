#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "shell.h"

int main(void)
{
    size_t buf_size = 0;
    char *buf = NULL;
    ssize_t read_chars;

    while (1)
    {
        write(1, "#cisfun$ ", 9);
        read_chars = getline(&buf, &buf_size, stdin);

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
        {
            buf[read_chars - 1] = '\0';
        }

        execute_command(buf);
       }
	return (0);
}

