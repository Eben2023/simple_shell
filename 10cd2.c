#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

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

    char *cwd;
    char *home = getenv("HOME");
    if (home == NULL)
    {
        perror("getenv error");
        exit(EXIT_FAILURE);
    }

    char *prev_cwd = NULL;

    while (1)
    {
        cwd = getcwd(NULL, 0);
        if (cwd == NULL)
        {
            perror("getcwd error");
            exit(EXIT_FAILURE);
        }
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
            char *token;
            int i = 0;
            token = strtok(buffer, " \n");
            while (token != NULL)
            {
                args[i++] = token;
                token = strtok(NULL, " \n");
            }
            args[i] = NULL;

            if (strcmp(args[0], "cd") == 0)
            {
                char *dir = args[1];
                if (dir == NULL || strcmp(dir, "~") == 0)
                {
                    dir = home;
                }
                else if (strcmp(dir, "-") == 0)
                {
                    if (prev_cwd == NULL)
                    {
                        fprintf(stderr, "No previous working directory\n");
                        continue;
                    }
                    dir = prev_cwd;
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
                    if (prev_cwd != NULL) free(prev_cwd);
                    prev_cwd = getcwd(NULL, 0);
                }
            }
            else if (strcmp(args[0], "exit") == 0)
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
        }
    }
}

free(buffer);
return 0;
}
