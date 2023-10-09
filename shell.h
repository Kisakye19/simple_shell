#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *read_line(void);
void handle_exit();
char **split_line(char *line);
int execute(char **args, char **environ);

#endif /* SHELL_H */

