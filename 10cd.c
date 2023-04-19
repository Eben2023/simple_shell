#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

#define BUFSIZE 1024

void print_error(char *message)
{
    fprintf(stderr, "%s: %s\n", message, strerror(errno));
}

void set_env(char *variable, char *value)
{
    if (setenv(variable, value, 1) == -1)
    {
        print_error("setenv error");
    }
}

void unset_env(char *variable)
{
    if (unsetenv(variable) == -1)
    {
        print_error("unsetenv error");
    }
}

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

    char *home_dir = getenv("HOME");
    if (home_dir == NULL)
    {
        print_error("getenv error");
        exit(EXIT_FAILURE);
    }

    char *current_dir = malloc(BUFSIZE * sizeof(char));
    if (current_dir == NULL)
    {
        print_error("malloc error");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        getcwd(current_dir, BUFSIZE);
        printf("%s$ ", current_dir);

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
            char *token;
            int i = 0;
            token = strtok(buffer, " \n");
            while (token != NULL)
            {
                args[i++] = token;
                token = strtok(NULL, " \n");
            }
            args[i] = NULL;

            char *envp[] = {NULL}; // empty environment variable list

            if (args[0] == NULL)
            {
                continue;
            }
            else if (strcmp(args[0], "exit") == 0)
            {
                if (args[1] == NULL)
                {
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    int status = atoi(args[1]);
                    exit(status);
                }
            }
            else if (strcmp(args[0], "cd") == 0)
            {
                if (args[1] == NULL)
                {
                    chdir(home_dir);
                }
                else if (strcmp(args[1], "-") == 0)
                {
                    char *previous_dir = getenv("OLDPWD");
                    if (previous_dir == NULL)
                    {
                        print_error("cd error");
                    }
                    else
                    {
                        chdir(previous_dir);
                        printf("%s\n", previous_dir);
                    }
                }
                else
                {
                    if (chdir(args[1]) == -1)
                    {
                        print_error("cd error");
                    }
                    else
                    {
                        set_env("OLDPWD", current_dir);
                    }
                }
            }
            else if (strcmp(args[0], "setenv") == 0)
            {
                if (args[1] == NULL || args[2] == NULL)
                {
                    print_error("setenv error");
                }
                else
                {
                    set_env(args[1], args[2]);
                }
            }
            else if (strcmp(args[0], "unsetenv") == 0)
            {
                if (args[1] == NULL)
                {
                    print_error("unsetenv error");
                }
                else
                {
                    unset_env(args[1]);
                }
            }
        }
    }
}
