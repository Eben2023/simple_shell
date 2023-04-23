#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

#define BUFSIZE 1024

char *read_input(void);
void parse_input(char *input, char **args);
void change_directory(char **args);
void exit_shell(char **args);



//Reading user input:
char *read_input(void)
{
    char *buffer = NULL;
    size_t bufsize = 0;
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

//Parsing user input:
void parse_input(char *input, char **args)
{
    char *token;
    int i = 0;
    token = strtok(input, " \n");
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL;
}

//Changing the current working directory:
void change_directory(char **args)
{
    char *home = getenv("HOME");
    if (home == NULL)
    {
        perror("getenv error");
        return;
    }

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
        if (errno == ENOENT) {
            fprintf(stderr, "%s: Directory not found\n", dir);
        } else {
            perror("chdir error");
        }
    }
    else
    {
        setenv("OLDPWD", getcwd(NULL, 0), 1);
    }
}

//Exiting the shell:
void exit_shell(char **args)
{
    int exit_status = 0;
    if (args[1] != NULL)
    {
        exit_status = atoi(args[1]);
    }
    exit(exit_status);
}

//Setting environment variables:
void set_environment_variable(char **args)
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

//Unsetting environment variables:
void unset_environment_variable(char **args)
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

//Executing external programs:
void execute_external_program(char **args)
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

// main.c
int main(void)
{
char *buffer;
size_t bufsize = BUFSIZE;
ssize_t nread;

buffer = (char *)malloc(bufsize * sizeof(char));
if (buffer == NULL)
{
    perror("malloc error");
    exit(EXIT_FAILURE);
}

while (1)
{
    char *cwd = get_cwd();
    printf("%s$ ", cwd);
    free(cwd);

    nread = getline(&buffer, &bufsize, stdin);
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
    else
    {
        char *args[BUFSIZE];
        int num_args = tokenize_input(buffer, args);

        if (num_args == 0) {
            continue;
        }

        if (is_builtin(args[0])) {
            execute_builtin(num_args, args);
        } else {
            execute_external_program(args);
        }
    }
}

free(buffer);
return 0;
}
