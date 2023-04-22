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
void execute_command(char **args)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // child process
        execvp(args[0], args);
        perror("execvp error");
        exit(EXIT_FAILURE);
    }
    else
    {
        // parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0)
            {
                fprintf(stderr, "Program exited with status %d\n", exit_status);
            }
        }
        else if (WIFSIGNALED(status))
        {
            int signal_number = WTERMSIG(status);
            fprintf(stderr, "Program was terminated by signal %d (%s)\n", signal_number, strsignal(signal_number));
        }
        else
        {
            fprintf(stderr, "Program exited abnormally\n");
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

        if (args[0] == NULL)
        {
            continue;
        }

        if (strcmp(args[0], "exit") == 0)
        {
            exit(0);
        }

        if (strcmp(args[0], "cd") == 0)
        {
            if (chdir(args[1]) != 0)
            {
                handle_error("cd failed");
            }
            continue;
        }

        if (strcmp(args[0], "set") == 0)
        {
            set_env_variable(args);
            continue;
        }

        if (strcmp(args[0], "unset") == 0)
        {
            unset_env_variable(args);
            continue;
        }

        execute_command(args);
    }

    free(buffer); // added this
    return 0;
}

