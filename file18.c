#include "main.h"

/**
 * epshell_loop - runs the main loop of epshell
 * @info: pointer to the dat_t struct
 * @av: command line arguments
 *
 * Return: 0 on success, -1 on failure
 */

static int epshell_loop(dat_t *info, char **av)
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
 * 	0 successful
 * 	1 found, not successful
 * 	2 exit
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



void cm_look_helper(dat_t *info);

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

/**
 * cm_look_helper - Helper function for cm_look to display error message
 * @info: Struct containing command information
 *
 * Description: This function displays an error message when a command is not found
 *
 * Return: (void)
 */

void cm_look_helper(dat_t *info)
{
    if ((interac_mode(info) || retrieve_env_func(info, "PATH=") || info->argv[0][0] == '/') && check_cm(info, info->argv[0]))
    {
        fork_func_look(info);
    }
    else if (*(info->arg) != '\n')
    {
        info->status = 127;
        disp_err_msg(info, "not found\n");
    }
}


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