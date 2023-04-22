#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define BUFSIZE 1024
#define MAX_BUFFER_SIZE 1024
#define MAX_ARGS 64

void read_input(char *buffer, size_t bufsize);
void parse_input(char *buffer, char **args);
void execute_command(char **args);
void handle_error(char *message);
void set_env_variable(char **args);
void unset_env_variable(char **args);
void display_prompt();

// Module 1: Input reading and parsing
// This module will handle reading input from the user and parsing
// it into separate arguments.
void read_input(char *buffer, size_t bufsize)
{
    ssize_t nread = getline(&buffer, &bufsize, stdin);
    if (nread == -1)
    {
        if (feof(stdin))
        {
            printf("\n");
            exit(EXIT_SUCCESS);
        }
        else
        {
            perror("getline error");
            exit(EXIT_FAILURE);
        }
    }
}

void parse_input(char *buffer, char **args)
{
    int i = 0;
    char *token = strtok(buffer, " \n");
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL;
}

// Module 2: Command execution
// This module will handle executing commands and creating child
// processes.
extern char **environ;

void execute_command(char **args)
{
    pid_t pid;
    int status;
    char *path;

    if (strcmp(args[0], "cd") == 0)
    {
        // Handle cd command
        if (args[1] == NULL)
        {
            // Change to home directory if no argument is given
            if (chdir(getenv("HOME")) == -1)
            {
                perror("cd");
            }
        }
        else if (strcmp(args[1], "-") == 0)
        {
            // Change to previous directory if "-" is given as argument
            char *oldpwd = getenv("OLDPWD");
            if (oldpwd == NULL)
            {
                fprintf(stderr, "OLDPWD environment variable not set\n");
            }
            else
            {
                if (chdir(oldpwd) == -1)
                {
                    perror("cd");
                }
                printf("%s\n", oldpwd);
            }
        }
        else
        {
            // Change to the specified directory
            if (chdir(args[1]) == -1)
            {
                perror("cd");
            }
        }

        // Update PWD and OLDPWD environment variables
        char *cwd = getcwd(NULL, 0);
        if (cwd == NULL)
        {
            perror("getcwd error");
            exit(EXIT_FAILURE);
        }
        setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", cwd, 1);
        free(cwd);
    }
    else
    {
        // Handle non-cd commands
        pid = fork();
        if (pid == 0)
        {
            // Child process
            if (args[0][0] == '/' || (args[0][0] == '.' && (args[0][1] == '/' || (args[0][1] == '.' && args[0][2] == '/'))))
            {
                // Execute the command using the given path
                if (execve(args[0], args, environ) == -1)
                {
                    perror("execve");
                    exit(EXIT_FAILURE);
                }
            }

            else if (strcmp(args[0], "cd") == 0)
            {
                // Handle cd command
                char *dir = args[1];
                if (dir == NULL)
                {
                    dir = getenv("HOME");
                    if (dir == NULL)
                    {
                        fprintf(stderr, "cd: HOME not set\n");
                        return;
                    }
                }
                else if (strcmp(dir, "-") == 0)
                {
                    dir = getenv("OLDPWD");
                    if (dir == NULL)
                    {
                        fprintf(stderr, "cd: OLDPWD not set\n");
                        return;
                    }
                    printf("%s\n", dir);
                }
                char cwd[BUFSIZE];
                if (getcwd(cwd, BUFSIZE) == NULL)
                {
                    perror("getcwd error");
                    return;
                }

                if (chdir(dir) == -1)
                {
                    perror("chdir error");
                    return;
                }

                if (setenv("OLDPWD", cwd, 1) == -1)
                {
                    perror("setenv error");
                    return;
                }

                char *newcwd = getcwd(NULL, 0);
                if (newcwd == NULL)
                {
                    perror("getcwd error");
                    return;
                }
                if (setenv("PWD", newcwd, 1) == -1)
                {
                    perror("setenv error");
                    return;
                }
                free(newcwd);
            }
            else if (strcmp(args[0], "setenv") == 0)
            {
                set_env_variable(args);
            }
            else if (strcmp(args[0], "unsetenv") == 0)
            {
                unset_env_variable(args);
            }
            else
            {
                execute_command(args);
            }
        }
    }
}

// Module 3: Error handling
// This module will handle error messages that are displayed to
// the user.
void handle_error(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

// Module 4: Environment variable handling
// This module will handle setting and unsetting environment
// variables.
void set_env_variable(char **args)
{
    if (args[1] == NULL || args[2] == NULL || args[3] != NULL)
    {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
    }
    else if (setenv(args[1], args[2], 1) == -1)
    {
        fprintf(stderr, "Failed to set environment variable\n");
    }
}

void unset_env_variable(char **args)
{
    if (args[1] == NULL || args[2] != NULL)
    {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
    }
    else if (unsetenv(args[1]) == -1)
    {
        fprintf(stderr, "Failed to unset environment variable\n");
    }
}

// Module 5: Prompt display
// This module will handle displaying the shell prompt.
void display_prompt()
{
    char *cwd = getcwd(NULL, 0);
    if (cwd == NULL)
    {
        perror("getcwd error");
        exit(EXIT_FAILURE);
    }
    printf("%s$ ", cwd);
    free(cwd);
}

// Module 6: Main function
// Description: This module contains the main function of the shell
// program. It calls the other modules to execute the different
// functionalities of the shell.
int main()
{
    char buffer[MAX_BUFFER_SIZE];
    char *args[MAX_ARGS];
    while (1)
    {
        display_prompt();
        read_input(buffer, MAX_BUFFER_SIZE);
        parse_input(buffer, args);
        execute_command(args);
    }

    return 0;
}
