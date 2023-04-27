#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
            // remove the newline character at the end
           // buffer[nread-1] = '\0';
	    char *args[] = {NULL};
            // execute the command
            if (execve(buffer, args, environ) == -1)
            {
                printf("command not found\n");
            }
        }
    }

    return 0;
}

