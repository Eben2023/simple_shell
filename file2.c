#include "main.h"

/**
 * term_exit - exits the shell
 * @info: contains potential arguments
 * Return: exits with a given exit status
 * (0) if info.argv[0] != "exit"
 */

int term_exit(dat_t *info)
{

if (info->argv[1]) /* If there is an exit arguement */
{

if ((my_atoi_alt(info->argv[1])) == -1)
{
info->status = 2;

disp_err_msg(info, "Wrong number: ");

myputs(info->argv[1]);

myputschar('\n');

return (1);
}
info->err_num = my_atoi_alt(info->argv[1]);
return (-2);
}
info->err_num = -1;

return (-2);
}

/**
 * help_me_func - function to switch directories
 * @info: contains arguments.
 * Return: Always 0
 */

int help_me_func(dat_t *info)
{
char **_args_;
_args_ = info->argv;

my_puts_func_("help me function not ready yet! \n");
if (0)
my_puts_func_(*_args_);
return (0);
}
