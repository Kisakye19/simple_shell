#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *read_line(void);
void handle_exit();
char **split_line(char *line);
int execute(char **args, char **environ);
ssize_t custom_getline(char **lineptr, size_t *n);
void execute_env();
int split_input_into_commands(char *input, char *commands[], int max_commands);

#endif /* SHELL_H */

