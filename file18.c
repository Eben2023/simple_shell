#include "main.h"

/**
 * epshell - entry point for epshell program
 * @info: pointer to the dat_t struct
 * @av: command line arguments
 *
 * Return: 0 on success, -1 on failure
 */

int epshell(dat_t *info, char **av)
{
int ret;

ret = epshell_loop(info, av);

return (ret);
}

/**
 * built_in_look - function that locates a builtin command
 * @info: return info
 *
 * Return: -1 if built_in_struct not found,
 * 0 successful
 * 1 found, not successful
 * 2 exit
 */

int built_in_look(dat_t *info)
{
int built_in_ret = -1;
int i;

bin_struct builtintbl[] = {
{"exit", term_exit},
{"env", environ_var_func},
{"help", help_me_func},
{"history", my_hist_func},
{"setenv", set_environ_func},
{"unsetenv", unset_environ_func},
{"cd", cd_impl_func},
{"alias", my_alias_funct},
{NULL, NULL}};

for (i = 0; builtintbl[i].bin_type; i++)
if (my_str_cmp(info->argv[0], builtintbl[i].bin_type) == 0)
{
info->line_count++;
built_in_ret = builtintbl[i].bin_func(info);
break;
}

return (built_in_ret);
}



