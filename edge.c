// This is a simple C shell program that mimics the Linux shell program
// It can execute commands with arguments and handle errors
// It uses the fork() and execvp() system calls to create child processes and run commands
// It also supports the exit command to terminate the shell

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 80 // The maximum length of a command
#define MAX_ARGS 10 // The maximum number of arguments

// A function to parse a command line into an array of arguments
// It returns the number of arguments
int parse_command(char *command, char *args[])
{
    int count = 0; // The number of arguments
    char *token; // A pointer to a token

    // Use strtok() to split the command by spaces
    token = strtok(command, " ");
    while (token != NULL && count < MAX_ARGS)
    {
        args[count] = token; // Store the token in the array
        count++; // Increment the argument count
        token = strtok(NULL, " "); // Get the next token
    }
    args[count] = NULL; // Terminate the array with a NULL pointer
    return count; // Return the number of arguments
}

// A function to execute a command with arguments
// It returns 0 if successful, -1 if an error occurs
int execute_command(char *args[])
{
    int status; // The status of the child process
    pid_t pid; // The process ID of the child process

    // Use fork() to create a child process
    pid = fork();
    if (pid < 0) // If fork() fails, print an error message and return -1
    {
        perror("fork");
        return -1;
    }
    else if (pid == 0) // If fork() succeeds and this is the child process, use execvp() to run the command
    {
        if (execvp(args[0], args) < 0) // If execvp() fails, print an error message and exit with code 1
        {
            perror(args[0]);
            exit(1);
        }
    }
    else // If fork() succeeds and this is the parent process, wait for the child process to finish and return its status
    {
        if (waitpid(pid, &status, 0) < 0) // If waitpid() fails, print an error message and return -1
        {
            perror("waitpid");
            return -1;
        }
        else // If waitpid() succeeds, return the status of the child process
        {
            return status;
        }
    }
}

// The main function of the shell program
int main(void)
{
    char command[MAX_LINE]; // The command line input
    char *args[MAX_ARGS]; // The array of arguments
    int num_args; // The number of arguments

    while (1) // Loop until the user enters exit or an error occurs
    {
        printf("cshell> "); // Print a prompt
        fflush(stdout); // Flush the output buffer

        if (fgets(command, MAX_LINE, stdin) == NULL) // Read a line from stdin and check for errors or end-of-file
        {
            if (feof(stdin)) // If end-of-file is reached, exit normally
            {
                printf("\n");
                exit(0);
            }
            else // If an error occurs, print an error message and exit with code 1
            {
                perror("fgets");
                exit(1);
            }
        }

        command[strcspn(command, "\n")] = '\0'; // Remove the trailing newline character from the command

        if (strcmp(command, "") == 0) // If the command is empty, continue the loop
        {
            continue;
        }

        num_args = parse_command(command, args); // Parse the command into arguments and get the number of arguments

        if (strcmp(args[0], "exit") == 0) // If the first argument is exit, exit normally
        {
            exit(0);
        }

        if (execute_command(args) < 0) // Execute the command and check for errors
        {
            fprintf(stderr, "Command execution failed\n"); // Print an error message if execution fails
        }
    }

    return 0; // Return 0 if everything goes well (this line should never be reached)
}
