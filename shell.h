#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
char *read_line(void);
char **split_line(char *line, char separator);
int execute(char **args, char **environ);
int execute_single_command(char **args);
ssize_t custom_getline(char **lineptr, size_t *n);

#endif /* SHELL_H */
