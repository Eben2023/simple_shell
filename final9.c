#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

#define BUFSIZE 1024

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
        printf("EbenPearlShell$ ");
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

            if (strcmp(args[0], "exit") == 0)
            {
                int exit_status = 0;
                if (args[1] != NULL)
                {
                    exit_status = atoi(args[1]);
                }
                exit(exit_status);
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
                    if (execvp(args[0], args) == -1)
                    {
                        printf("command not found\n");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    // parent process
                    wait(NULL);
                }
            }
        }
    }

    return 0;
}

