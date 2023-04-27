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

        // tokenize input into arguments
        char *token;
        char *args[BUFSIZE];
        int i = 0;
        token = strtok(buffer, " \n");
        while (token != NULL)
        {
            args[i++] = token;
            token = strtok(NULL, " \n");
        }
        args[i] = NULL;

        // check for built-in commands
        if (strcmp(args[0], "exit") == 0)
        {
            exit(EXIT_SUCCESS);
        }

        // fork a child process to execute command
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
                perror("execvp error");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            // parent process
            wait(NULL);
        }
    }

    free(buffer);
    return 0;
}

