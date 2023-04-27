#include "main.h"

/**
 * cm_look - Looks for a command in the PATH and executes it if found
 * @info: Struct containing command information
 *
 * Description: This function searches for a command in the PATH and
 * executes it if found. If not, it displays an error message.
 *
 * Return: (void)
 */

void cm_look(dat_t *info)
{
char *path = NULL;
int i, k;

info->path = info->argv[0];

if (info->linecount_flag)
{
info->line_count++;
info->linecount_flag = 0;
}

for (i = 0, k = 0; info->arg[i]; i++)
{
if (!check_delim_(info->arg[i], " \t\n"))
k++;
}

if (!k)
return;

path = path_search(info, retrieve_env_func(info, "PATH="), info->argv[0]);

if (path)
{
info->path = path;
fork_func_look(info);
}
else
{
cm_look_helper(info);
}
}
