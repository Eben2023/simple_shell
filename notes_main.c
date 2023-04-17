#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>

/**
 * The variable "**environ" of type char has been already defined in another
 * file or library somewhere in the linux operating system. We will need the
 * environ variable later in the program
*/
extern char **environ;

#define BUFSIZE 1024

int main(void)
{
    char *buffer;
    size_t bufsize = BUFSIZE;
    ssize_t nread;

    /**
     * Malloc is called to allocate memory for the variable buffer
    */
    buffer = (char *)malloc(bufsize * sizeof(char));

    /**
     * If (buffer == NULL) means that if malloc fails to allocate memory
     * to the variable called buffer, the below code should be run.
     * Perror is a system function that prints a message to stdin and
     * also return a technical error message to the operating system.
     * That is why perror function was used instead of printf because the
     * printf function only prints a message and does nothing afterwards
    */
    if (buffer == NULL)
    {
        /**
         * perror function is called to print out the message "malloc error"
         * and also sends an error report to the operating system for
         * debuggin purposes
        */
        perror("malloc error");

        /**
         * exit function is called to terminate the program since our
         * malloc couldn't allocate memory
        */
        exit(EXIT_FAILURE);
    }

    /**
     * while (1) is an infinite loop because the value 1 represents true so
     * the block of code within the while loop will continue running until
     * the code exits with either EXIT_SUCCESS or EXIT_FAILURE
    */
    while (1) //or "for(;;)" //while(TRUE) after declaring TRUE to = 1
    {
        /**
         * When the code get to this point it means that molloc was able
         * to allocate memory space to buffer. Then the printf() function
         * will print the message on the terminal to serve as a prompt
         * for the user of our shell program to input a command
        */
        printf("EbenPearlShell$ ");

        /**
         * After "EbenPearlShell$" is printed as a prompt in the terminal,
         * the getline function is called to read the command inputed by
         * the user of our shell program
         * @buffer: this contains the reference to the variable that stores
         * the user input or commands.
         * @bufsize: Contains the number of characters read by the getline
         * function
         * @stdin: this is where the input will be taken from which is the
         * standard input
         * So in the end, the variable nread of type ssize_t will contain
         * the total bytes of the user command entered into our shell
        */
        nread = getline(&buffer, &bufsize, stdin);

        /**
         * nread == -1 checks if there was an error reading command input
         * feof(stdin) means the function "feof" checks if end of file (EOF)
         * has been reached. In this case when a user inputs the ctrl+D
         * command, our shell program will terminate successfully
        */
        if (nread == -1)
        {
            if (feof(stdin))
            {
                printf("\n");
                exit(EXIT_SUCCESS);
            }

            /**
             * if feof function did not reach the end of file, then something
             * else is responsible for the program exit. Therefore we use the
             * perror function to print out the message "getline error" which
             * also returns an error message to stderr function. This indicates
             * a failure termination of our shell program
            */
            else
            {
                perror("getline error");
                exit(EXIT_FAILURE);
            }
        }

        /**
         * Now if nread is not equal to -1, we can now go ahead and execute the
         * command that has been inputed after the prompt($) using the execve
         * function.
        */
        else
        {
            /**
             * char *args[] = {NULL} initializes an array of char pointers named
             * args with the value NULL. This array will contain the arguments
             * passed to the command being executed. For example, if ls is the
             * command, -l will be the argument that will be stored in args
            */
            char *args[] = {NULL};
            
            /**
             * The execve function is used to execute the command in the buffer
             * variable. The args aray is passed as the second argument to the
             * execve function and environ is passed as the third argument.
             * 
             * so if execve is == -1, it means that the execve function encountered
             * error while trying to execute that command.
            */
            if (execve(buffer, args, environ) == -1)
            {
                /*perror function would be preferrable to printf*/
                printf("command not found\n");
            }
        }
    }

    return 0;
}
