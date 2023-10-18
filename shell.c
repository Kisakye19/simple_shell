#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

extern char **environ; 
int is_numeric(const char *str);

/*int execute(char **args) {
    int status = 0;
    char **command_tokens = split_line(args[0]);
    int i = 0;

    while (command_tokens[i] != NULL) {
        status = execute_single_command(command_tokens + i);
        i++;
    }

    return status;
}*/

void handle_cd(char **args) {
    char *new_dir;
    char *current_dir = getcwd(NULL, 0);
    char *home = getenv("HOME");

    if (args[1] == NULL || strcmp(args[1], "~") == 0) {
        new_dir = home;
    } else if (strcmp(args[1], "-") == 0) {
        char *previous_dir = getenv("OLDPWD");
        if (previous_dir == NULL) {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return;
        }
        new_dir = previous_dir;
    } else {
        new_dir = args[1];
    }

    if (chdir(new_dir) != 0) {
        perror("cd");
    } else {
        /* Update PWD and OLDPWD environment variables*/
        setenv("PWD", getcwd(NULL, 0), 1);
        setenv("OLDPWD", current_dir, 1);
    }

    free(current_dir);
}

int handle_setenv(char **args) 
	{
    if (args[1] == NULL || args[2] == NULL) 
	{
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
        return 1;
    }
    
    if (setenv(args[1], args[2], 1) != 0) 
	{
        perror("setenv");
        return 1;
   }
    
    return 0;
}

int handle_unsetenv(char **args) 
	{
    if (args[1] == NULL) 
	{
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
        return 1;
    }
    
    if (unsetenv(args[1]) != 0) 
	{
        perror("unsetenv");
        return 1;
    }    
    return 0;
}


int handle_exit(char **args) 
{

    if (args[1] == NULL)
        return -1; 
    else if (is_numeric(args[1]))
        return atoi(args[1]); /*Exit the shell with a specific status code*/
    else
    {
        fprintf(stderr, "Usage: exit [status]\n");
        return 0;
    }

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

int is_numeric(const char *str)
{
    while (*str)
    {
        if (*str < '0' || *str > '9')
            return 0;
        str++;
    }
    return 1;
}

int main(void)
{
    char *line;
    char **args;
    int status, i, j;
    char *line_ptr;
    char *separator_pos;
    int result1;
    char cwd[PATH_MAX]; /* To store the current working directory*/

    while (1)
    {
	getcwd(cwd, sizeof(cwd)); /* Get the current working directory*/
        printf("($) %s$ ", cwd); /* Display the current directory in the prompt*/
        line = read_line();

	line_ptr = line;

        while ((separator_pos = strchr(line_ptr, ';')) != NULL)
        {
            *separator_pos = '\0'; /* Null-terminate the command*/
            args = split_line(line_ptr, ' '); /* Split the command*/
            line_ptr = separator_pos + 1; /* Move to the next command*/
            status = execute(args, environ); /* Execute the command*/
            free(args);
        }
	
	args = split_line(line_ptr, ' ');

	 if (args[0] != NULL)
        {
            int operator_index = -1;

            for (i = 0; args[i]; i++)
            {
                if (strcmp(args[i], "&&") == 0 || strcmp(args[i], "||") == 0)
                {
                    operator_index = i;
                    break;
                }
            }

            if (operator_index != -1)
            {
                char **command1 = malloc((operator_index + 1) * sizeof(char *));
                char **command2 = malloc((i - operator_index) * sizeof(char *));

                for (j = 0; j < operator_index; j++)
                    command1[j] = args[j];
                command1[operator_index] = NULL;

                result1 = execute(command1, environ);

                for (j = operator_index + 1; args[j]; j++)
                    command2[j - operator_index - 1] = args[j];
                command2[i - operator_index - 1] = NULL;

                if (strcmp(args[operator_index], "&&") == 0)
                {
                    if (result1 == 0)
                        execute(command2, environ);
                }
                else if (strcmp(args[operator_index], "||") == 0)
                {
                    if (result1 != 0)
                        execute(command2, environ);
                }

                free(command1);
                free(command2);
            }

	    if (strcmp(args[0], "cd") == 0) 
	    {
                handle_cd(args);
            }
		else if (strcmp(args[0], "exit") == 0) 
	    {
                 status = handle_exit(args);
                if (status == -1)
		break; /* Exit the shell*/
 		else
                    exit(status);
            } 
		else if (strcmp(args[0], "env") == 0) 
	    {
                execute_env(); /* Call the env command*/
            } 
		else if (strcmp(args[0], "setenv") == 0) 
	    {
                status = handle_setenv(args);
            } 
		else if (strcmp(args[0], "unsetenv") == 0) 
	    {
                status = handle_unsetenv(args);
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


char **split_line(char *line, char separator)
{
    int bufsize = 1024; /* Initial buffer size */
    int position = 0;
    char **tokens = NULL;
    char *token;

    if (!line)
    {
        perror("Error: Unable to read the command line\n");
        return NULL;
    }

    tokens = malloc(bufsize * sizeof(char *));
    if (!tokens)
    {
        perror("Error: Unable to allocate memory for tokens\n");
        exit(EXIT_FAILURE);
    }
    while (*line)
    {
        while (*line == ' ' || *line == '\t')
            line++;
    

        token = line;

	if (strncmp(line, "&&", 2) == 0 || strncmp(line, "||", 2) == 0)
        {
            line += 2;
            *line = '\0';  /* Null-terminate the operator*/
            line += 1;     /* Move to the next command*/
        } else {

        while (*line && *line != separator)
            line++;
	}

        if (*line) {
            *line++ = '\0';
	}

        tokens[position] = token;
        position++;
     if (position >= bufsize)
        {
            bufsize += 1024;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                perror("Error: Unable to reallocate memory for tokens\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    tokens[position] = NULL;
    return tokens;
}


int execute(char **args, char **environ)
{
    pid_t pid;
    int status = 0;

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

    return (0);
}
