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
            while (token != NULL) {
                args[i++] = token;
                token = strtok(NULL, " \n");
            }
            args[i] = NULL;

            char *envp[] = {NULL}; // empty environment variable list

            pid_t pid = fork();

            if (pid == -1)
            {
                perror("fork error");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                // child process
                if (strcmp(args[0], "ls") == 0) {
                    char *full_path = "/bin/ls"; // full path of the command
                    args[0] = full_path; // replace the first argument with full path
                }
                if (execve(args[0], args, envp) == -1)
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

    return 0;
}

