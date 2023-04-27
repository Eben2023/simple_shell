#include "main.h"

/**
 * cd_impl_func - function to switch directories
 * @info: contains arguments
 * Return: Always 0
 */

int cd_impl_func(dat_t *info)
{
int o;
char b[1024];
char *v;
char *chwd_var;
v = getcwd(b, 1024);
if (!v)
my_puts_func_("TODO: >>getcwd failure emsg here<<\n");
if (!info->argv[1])
{
chwd_var = retrieve_env_func(info, "HOME=");
if (!chwd_var)
o = chdir((chwd_var = retrieve_env_func(info, "PWD=")) ? chwd_var : "/");
else
o = chdir(chwd_var);
}
else if (my_str_cmp(info->argv[1], "-") == 0)
{
if (!retrieve_env_func(info, "OLDPWD="))
{
my_puts_func_(v);
my_putc_char('\n');
return (1);
}
my_puts_func_(retrieve_env_func(info, "OLDPWD=")), my_putc_char('\n');
o = chdir((chwd_var = retrieve_env_func(info, "OLDPWD=")) ? chwd_var : "/");
}
else
o = chdir(info->argv[1]);
if (o == -1)
{
disp_err_msg(info, "can't cd to ");
myputs(info->argv[1]), myputschar('\n');
}
else
{
my_set_environ_func_(info, "OLDPWD", retrieve_env_func(info, "PWD="));
my_set_environ_func_(info, "PWD", getcwd(b, 1024));
}
return (0);
}
