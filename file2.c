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
 * cd_impl_func - function to switch directories
 * @info: contains arguments
 * Return: Always 0
 */

int cd_impl_func(dat_t *info)
{
    char *cwd, *dir, buffer[1024];
    int chdir_ret;

    cwd = getcwd(buffer, 1024);
    if (!cwd)
    {
        my_puts_func_("TODO: >>getcwd failure emsg here<<\n");
    }

    if (!info->argv[1])
    {
        dir = retrieve_env_func(info, "HOME=");
        if (!dir)
        {
            dir = retrieve_env_func(info, "PWD=");
            if (!dir)
            {
                my_puts_func_("cd: HOME and PWD not set\n");
                return (1);
            }
        }
        chdir_ret = chdir(dir);
    }
    else if (my_str_cmp(info->argv[1], "-") == 0)
    {
        dir = retrieve_env_func(info, "OLDPWD=");
        if (!dir)
        {
            my_puts_func_(cwd);
            my_putc_char('\n');
            return (1);
        }
        my_puts_func_(dir);
        my_putc_char('\n');
        chdir_ret = chdir(dir);
    }
    else
    {
        chdir_ret = chdir(info->argv[1]);
    }

    if (chdir_ret == -1)
    {
        disp_err_msg(info, "can't cd to ");
        myputs(info->argv[1]), myputschar('\n');
    }
    else
    {
        my_set_environ_func_(info, "OLDPWD", cwd);
        my_set_environ_func_(info, "PWD", getcwd(buffer, 1024));
    }
    return (0);
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