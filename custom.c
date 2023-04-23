#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 100

// Function to read input from the command line
char *read_input() {
    static char input_buffer[MAX_INPUT_SIZE];
    static int input_buffer_pos = 0;
    if (input_buffer_pos == 0) {
        if (fgets(input_buffer, MAX_INPUT_SIZE, stdin) == NULL)
            return NULL;
        input_buffer[strlen(input_buffer) - 1] = '\0';
    }
    return &input_buffer[input_buffer_pos++];
}

// Function to parse input into tokens
char **parse_input(char *input) {
    static char *tokens[MAX_TOKENS];
    static int num_tokens = 0;
    char *token;
    num_tokens = 0;

    token = strtok(input, " ");
    while (token != NULL) {
        tokens[num_tokens] = token;
        num_tokens++;
        token = strtok(NULL, " ");
    }
    tokens[num_tokens] = NULL;
    return tokens;
}

// Function to execute a command
int execute_command(char **tokens) {
    int status;
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execve(tokens[0], tokens, NULL);
        if (errno == ENOENT) {
            printf("%s: command not found\n", tokens[0]);
            exit(EXIT_FAILURE);
        } else {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return -1;
        }
    }
}

// Function to handle the cd command
void change_directory(char **tokens) {
    if (tokens[1] == NULL) {
        // No argument provided, change to the home directory
        chdir(getenv("HOME"));
    } else if (strcmp(tokens[1], "-") == 0) {
        // Change to the previous directory
        chdir(getenv("OLDPWD"));
    } else {
        // Change to the specified directory
        if (chdir(tokens[1]) == -1) {
            perror(tokens[1]);
        }
    }
}

// Function to print the environment variables
void print_environment() {
    extern char **environ;
    char **env = environ;
    while (*env) {
        printf("%s\n", *env);
        env++;
    }
}

// Function to handle the exit command
void exit_shell(char **tokens) {
    if (tokens[1] != NULL) {
        exit(atoi(tokens[1]));
    } else {
        exit(0);
    }
}

// Function to handle the setenv command
void set_environment_variable(char **tokens) {
    if (tokens[1] == NULL || tokens[2] == NULL) {
        printf("Usage: setenv VARNAME VALUE\n");
    } else {
        setenv(tokens[1], tokens[2], 1);
    }
}

// Function to handle the unsetenv command
void unset_environment_variable(char **tokens) {
    if (tokens[1] == NULL) {
        printf("Usage: unsetenv VARNAME\n");
    } else {
        unsetenv(tokens[1]);
    }
}

int main() {
    char *input;
    char **tokens;
    int status;
do {
    printf("> ");
    input = read_input();
    tokens = parse_input(input);

    if (tokens[0] != NULL) {
        if (strcmp(tokens[0], "cd") == 0) {
            change_directory(tokens);
        } else if (strcmp(tokens[0], "exit") == 0) {
            exit_shell(tokens);
        } else if (strcmp(tokens[0], "env") == 0) {
            print_environment();
        } else if (strcmp(tokens[0], "setenv") == 0) {
            set_environment_variable(tokens);
        } else if (strcmp(tokens[0], "unsetenv") == 0) {
            unset_environment_variable(tokens);
        } else {
            // Check if the command exists in the PATH environment variable
            char *path = getenv("PATH");
            char *path_copy = strdup(path);
            char *dir = strtok(path_copy, ":");
            while (dir != NULL) {
                char command_path[MAX_INPUT_SIZE];
                snprintf(command_path, MAX_INPUT_SIZE, "%s/%s", dir, tokens[0]);
                if (access(command_path, X_OK) == 0) {
                    tokens[0] = command_path;
                    break;
                }
                dir = strtok(NULL, ":");
            }
            free(path_copy);

            // Execute the command
            status = execute_command(tokens);
        }
    }
} while (status != -1);
return 0;
}
