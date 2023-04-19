#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 1024

int main(void)
{
    char *buffer = NULL;
    size_t bufsize = 0;
    ssize_t nread;

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

            pid_t pid = fork();

            if (pid == -1)
            {
                perror("fork error");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                // child process
                if (strcmp(args[0], "exit") == 0)
                {
                    free(buffer);
                    exit(EXIT_SUCCESS);
                }
                else if (strcmp(args[0], "cd") == 0)
                {
                    if (args[1] == NULL)
                    {
                        chdir(getenv("HOME"));
                    }
                    else
                    {
                        if (chdir(args[1]) != 0)
                        {
                            printf("cd: %s: No such file or directory\n", args[1]);
                        }
                    }
                    free(buffer);
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    execvp(args[0], args);
                    printf("command not found\n");
                    free(buffer);
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

    return 0;
}

