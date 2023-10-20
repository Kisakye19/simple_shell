#ifndef SHELL_H
#define SHELL_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>

/*getline*/
ssize_t custom_getline(char **lineptr, size_t *n);

/* environment variables */
extern char **environ;
extern __sighandler_t signal(int __sig, __sighandler_t __handler);

/* handle built ins */
int custom_checker(char **arg, char *buf);
void prompt_use(void);
void handle_signal(int m);
char **tokenize(char *line);
char *test_path(char **path, char *args);
char *custom_append_paths(char *path, char *args);
int handle_built_in(char **args, char *line);
void exit_arg(char **args, char *line);

void print_env(void);

/* string handlers */
int _strcmp(char *s1, char *s2);
int _strlen(char *s);
int _strncmp(char *s1, char *s2, int n);
char *_strdup(char *s);
char *_strchr(char *s, char c);

void execution(char *cp, char **arg);
char *find_paths(void);

/* helper function for efficient free */
void free_buffer(char **buf);

struct builtin
{
	char *env;
	char *exit;
} builtin;

struct info
{
	int final_exit;
	int ln_count;
} info;

struct flags
{
	bool interactive;
} flags;

#endif /* SHELL_H */
