#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

#define MAX_ARGS 64

void print_prompt(char *cwd);
char *read_command(char *buffer, size_t bufsize);
char **parse_command(char *buffer);
void execute_command(char **args, char *cwd, char *home);
void handle_cd(char **args, char *home, char *cwd);
void handle_exit(char **args);
void shell_cd(char **args, char *home);
void shell_exit(char **args);
void shell_execute(char **args);
void handle_exec(char **args);
void shell_setenv(char **args);

void print_prompt(char *cwd)
{
    printf("%s$ ", cwd);
}

char *read_command(char *buffer, size_t bufsize)
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
    return buffer;
}

char **parse_command(char *buffer)
{
    char **args = (char **)malloc(MAX_ARGS * sizeof(char *));
    if (args == NULL)
    {
        perror("malloc error");
        exit(EXIT_FAILURE);
    }

    char *token;
    int i = 0;
    token = strtok(buffer, " \n");
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL;

    return args;
}

void execute_command(char **args, char *cwd, char *home)
{
    if (strcmp(args[0], "cd") == 0)
    {
        handle_cd(args, home, cwd);
    }
    else if (strcmp(args[0], "setenv") == 0)
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
    else if (strcmp(args[0], "unsetenv") == 0)
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
    else if (strcmp(args[0], "exec") == 0)
    {
        handle_exec(args);
    }
    else
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
            handle_exec(args);
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
}

void handle_cd(char **args, char *home, char *cwd)
{
    shell_cd(args, home);
}

void handle_exit(char **args)
{
    shell_exit(NULL);
}

void handle_setenv(char **args)
{
    shell_setenv(args);
}

void handle_unsetenv(char **args)
{
    shell_unsetenv(args);
}

void handle_exec(char **args)
{
    execvp(args[0], args);
    perror("execvp error");
    exit(EXIT_FAILURE);
}

void shell_cd(char **args, char *home)
{
    char *dir = args[1];
    if (dir == NULL || strcmp(dir, "~") == 0)
    {
        dir = home;
    }
    else if (strcmp(dir, "-") == 0)
    {
        dir = getenv("OLDPWD");
        if (dir == NULL)
        {
            dir = home;
        }
        printf("%s\n", dir);
    }
    if (chdir(dir) == -1)
    {
        if (errno == ENOENT)
        {
            fprintf(stderr, "%s: Directory not found\n", dir);
        }
        else
        {
            perror("chdir error");
        }
    }
    else
    {
        setenv("OLDPWD", cwd, 1);
    }
}

void shell_exit(char **args)
{
    int exit_status = 0;
    if (args != NULL && args[1] != NULL)
    {
        exit_status = atoi(args[1]);
    }
    exit(exit_status);
}

void shell_setenv(char **args)
{
    if (args[1] == NULL || args[2] == NULL || args[3] != NULL)
    {
        fprintf(stderr, "Usage: if (args[1] == NULL || args[2] == NULL || args[3] != NULL")
            {
            fprintf(stderr, "Usage: %s input_file output_file\n", args[0]);
            return 1;
            }
    }
}
