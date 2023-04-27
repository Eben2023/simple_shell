#include "main.h"

/**
 * cm_look_helper - Helper function for cm_look to display error message
 * @info: Struct containing command information
 *
 * Description: This function displays an error message when a command is
 * not found
 *
 * Return: (void)
 */

void cm_look_helper(dat_t *info)
{
if ((interac_mode(info) || retrieve_env_func(info, "PATH=") ||
info->argv[0][0] == '/') && check_cm(info, info->argv[0]))
{
fork_func_look(info);
}
else if (*(info->arg) != '\n')
{
info->status = 127;
disp_err_msg(info, "not found\n");
}
}
