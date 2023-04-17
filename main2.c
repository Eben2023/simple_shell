#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
        printf("EbenPearl$ ");
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

            if (execve(args[0], args, envp) == -1)
            {
                printf("command not found\n");
            }
        }
    }

    return 0;
}

