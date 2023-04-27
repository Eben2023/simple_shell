#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

#define BUFSIZE 1024

int main(void)
{
    setenv("PATH", "/bin", 1); // add /bin to PATH

    char *buffer;
    size_t bufsize = BUFSIZE;
    ssize_t nread;

    buffer = (char *)malloc(bufsize * sizeof(char));
    if (buffer == NULL)
    {
        perror("malloc error");
        exit(EXIT_FAILURE);
    }

    setenv("PATH", "/bin", 1);

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

            if (strcmp(args[0], "exit") == 0) // check if command is exit
            {
                exit(EXIT_SUCCESS);
            }

            pid_t pid = fork();

            if (pid == -1)
            {
                perror("fork error");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                // child process
                if (execve(args[0], args, environ) == -1)
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

