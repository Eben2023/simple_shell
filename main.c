#include "main.h"

/**
 * modify_fd - modify file descriptor value using inline assembly
 * @fd: pointer to file descriptor value
 *
 * Return: void
 */
void modify_fd(int *fd)
{
asm("mov %1, %0\n\t"
"add $3, %0"
: "=r"(*fd)
: "r"(*fd));
}

/**
 * open_file - open specified file and return file descriptor
 * @filename: name of file to open
 *
 * Return: file descriptor on success, -1 on failure
 */
int open_file(char *filename)
{
return (open(filename, O_RDONLY));
}

/**
 * handle_open_error - handle error that occurs when opening a file
 * @program_name: name of program (argv[0])
 * @filename: name of file that failed to open
 *
 * Return: void
 */
void handle_open_error(char *program_name, char *filename)
{
if (errno == EACCES)
{
exit(126);
}
if (errno == ENOENT)
{
myputs(program_name);
myputs(": 0: Can't open ");
myputs(filename);
myputschar('\n');
myputschar(FL_BUFFER_CONST);
exit(127);
}
exit(EXIT_FAILURE);
}

/**
 * run_shell - run the shell
 * @info: pointer to dat_t struct containing shell information
 * @av: command line arguments
 *
 * Return: void
 */
void run_shell(dat_t *info, char **av)
{
epshell(info, av);
}

/**
 * main - entry point for program
 * @ac: number of command line arguments
 * @av: array of command line arguments
 *
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int main(int ac, char **av)
{
dat_t info[] = {INITIALIZE_DATA};
int fd = 2;

modify_fd(&fd);

if (ac == 2)
{
fd = open_file(av[1]);
if (fd == -1)
{
handle_open_error(av[0], av[1]);
}
info->readfd = fd;
}

set_environ_data(info);
get_histreg_data(info);
run_shell(info, av);

return (EXIT_SUCCESS);
}
