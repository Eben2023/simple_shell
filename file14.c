#include "main.h"

/**
 * fork_func_look - creates a child process and executes a program.
 * @info: pointer to a struct containing path, arguments and
 * environment variables.
 *
 * Description: fork_func_look() executes a program using a child
 * process, waits for it to complete and returns its exit status,
 * with an error message printed to standard error if any error occurs.
 *
 * Return: This function does not return a value.
 */

void fork_func_look(dat_t *info)
{
pid_t child_pid;

child_pid = fork();
if (child_pid == -1)
{
perror("Error:");
return;
}
if (child_pid == 0)
{
if (execve(info->path, info->argv, my_retr_environ_func(info)) == -1)
{
release_data_func(info, 1);
if (errno == EACCES)
exit(126);
exit(1);
}
perror("Error:");
return;
}
else
{
waitpid(child_pid, &(info->status), 0);
if (WIFEXITED(info->status))
{
info->status = WEXITSTATUS(info->status);
if (info->status == 126)
disp_err_msg(info, "Permission denied\n");
}
}
}
