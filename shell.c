#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ; 

void handle_exit() 
{
	printf("Exiting the shell\n");
	exit(0); 
}

void execute_env() 
{
	char **env = environ;
	while (*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
}

int main(void)
{
	char *line;
	char **args;
	int status;
	int arg_count = 0;

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
			else if (strcmp(args[0], "env") == 0)
			{
				execute_env(); /* Call the env command*/
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
if (arg_count > 0)
{
	/*Implemanting exit*/
	if (strcmp(args[0], "exit") == 0)
	{
		if (arg_count > 1)
		{
			/*If There's an argument after the exit (Success)*/
			int exit_status = atoi(args[1]);
			exit(exit_status);
		}
		else
		{
			/*If there's no argument, exit with defult stat 0*/
			exit(0);
		}
	}
	else if (strcmp(args[0], "setenv") == 0)
	{
		/*checks for the setenv built-in command*/
		if (arg_count != 3)
		{
			fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		}
		else
		{
			if (setenv(args[1], args[2], 1) != 0)
			{
				fprintf(stderr, "Failed to set variable.\n");
			}
		}
	}
	else if (strcmp(args[0], "unsetenv") != 0)
	{
		/*checks for the unsetenv built-in command*/
		if (arg_count != 2)
		{
			fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		}
		else
		{
			if (unsetenv(args[1]) != 0)
			{
				fprintf(stderr, "failed to unset environment variable.\n");
			}
		}
	}
		 else if (strcmp(args[0], "cd") == 0)
		{
			/*checks for the cd built-in command*/
			if (arg_count == 1)
			{
				char *home = getenv("HOME");
				if (home != NULL)
				{
					if (chdir(home) != 0)
					{
						fprintf(stderr, "Failed to change directory.\n");
					}
					else
					{
						/*Updates the PWD command*/
					if (getcwd(current_directory, sizeof(current_directory)) != NULL)
					{
						setenv("PWD", current_directory, 1);
					}
					}
				}
			}
			else
			{
				fprintf(stderr, "HOME environment variable not set.\n");
			}
		}
	else if (arg_count == 2)
	{
		if (strcmp(args[1], "-") == 0)
		{
			/*HANDLE "cd -"*/
			char *previous_directory = getenv("OLDPWD");
			
			if (previous_directory != NULL)
			{
				if (chdir(previous_directory) != 0)
				{
					fprintf(stderr, "Failed to change directory.\n");
				}
				else
				{
					/*pwd & oldpwd update*/
					char current_directory[PATH_MAX];
					if (getcwd(current_directory, sizeof(current_directory)) != NULL)
					{
						setenv("OLDPWD", current_directory, 1);
						setenv("PWD", previous_directory, 1);
					}
				}
			}
			else
			{
				/*changes dir to the specified path*/
				if (chdir(args[1]) != 0)
				{
					fprintf(stderr, "Failed to change directory.\n");
				}
				else
				{
					/*PWD environment var update*/
					char current_directory[PATH_MAX];
				
					if (getcwd(current_directory, sizeof(current_directory)) != NULL)
					{
						setenv("PWD", current_directory, 1);
					}
				}
			}
		}
		else
		{
			fprintf(stderr, "Usage: cd [DIRECTORY]\n");
		}
	}


/**
 * splint_input_into_commands - A function that handles the commands separator
 */

int split_input_into_commands(char *input, char *commands[], int max_commands)
{
	int i = 0;
	int command_index = 0;
	int in_command = 0;

	while (input[i] != '\0')
	{
		if (input[i] == ';')
		{
			if (in_command)
			{
				input[i] = '\0';
				commands[command_index] = input + i - in_command;
				command_index++;
				in_command = 0;
			}
			i++;
		}

		if (command_index >= max_commands)
		{
			break;
		}
	}

	if (in_command)
	{
		commands[command_index] = input + i - in_command;
		command_index++;
	}

	return command_index;
}

/* end of handling commands */

char *read_line(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	nread = custom_getline(&line, &len);
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
		}
		else
		{
			/* If the command doesn't contain a path, search in PATH */
			char *path = getenv("PATH");
			char *token, *full_path, *dirs, *saveptr;
            
			if (path == NULL)
			{
				perror("No PATH found in environment");
				exit(EXIT_FAILURE);
			}
			dirs = strdup(path);
			if (dirs == NULL)
			{
				perror("strdup");
				exit(EXIT_FAILURE);
			}

			token = strtok_r(dirs, ":", &saveptr);
			while (token != NULL)
			{
				full_path = malloc(strlen(token) + strlen(args[0]) + 2);
				if (full_path == NULL)
				{
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


