#include "main.h"

/**
 * epshell_loop - runs the main loop of epshell
 * @info: pointer to the dat_t struct
 * @av: command line arguments
 *
 * Return: 0 on success, -1 on failure
 */

int epshell_loop(dat_t *info, char **av)
{
ssize_t r = 0;
int builtin_ret = 0;

while (r != -1 && builtin_ret != -2)
{
del_data_func(info);
if (interac_mode(info))
my_puts_func_("$ ");
myputschar(FL_BUFFER_CONST);
r = my_read_input_func(info);
if (r != -1)
{
reg_data_func(info, av);
builtin_ret = built_in_look(info);
if (builtin_ret == -1)
cm_look(info);
}
else if (interac_mode(info))
my_putc_char('\n');
release_data_func(info, 0);
}

reg_history_data_func(info);
release_data_func(info, 1);

if (!interac_mode(info) && info->status)
exit(info->status);

if (builtin_ret == -2)
{
if (info->err_num == -1)
exit(info->status);
exit(info->err_num);
}

return (builtin_ret);
}
