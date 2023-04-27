Write a function that converts a binary number to an unsigned int.

Prototype: unsigned int binary_to_uint(const char *b);
where b is pointing to a string of 0 and 1 chars
Return: the converted number, or 0 if
there is one or more chars in the string b that is not 0 or 1
b is NULL#include "main.h"

/**
 * del_data_func - set dat_t to zero
 * @info: address
 */

void del_data_func(dat_t *info)
{
    info->arg = NULL;
    info->argv = NULL;
    info->path = NULL;
    info->argc = 0;
}

/**
 * reg_data_func - sets struct values to 0
 * @info: address for struct
 * @av: para or argument
 */

void reg_data_func(dat_t *info, char **av)
{
    int a;

    info->fname = av[0];
    info->argv = my_str_tw_func(info->arg, " \t");
    if (!info->argv)
    {
        info->argv = malloc(sizeof(char *) * 2);
        if (info->argv)
        {
            info->argv[0] = mystring_dup(info->arg);
            info->argv[1] = NULL;
        }
    }
    for (a = 0; info->argv && info->argv[a]; a++)
        ;
    info->argc = a;

    checkout_alias_func(info);
    checkout_variable_func_(info);
}


/**
 * release_data_func - frees memory and resets variables in dat_t struct
 * @info: pointer to dat_t struct
 * @all: flag to indicate if all memory should be freed
 *
 * Return: void
 */
void release_data_func(dat_t *info, int all)
{
    free_func(info->argv);
    info->argv = NULL;
    info->path = NULL;

    if (all)
    {
        if (!info->cmd_buf)
            free(info->arg);

        _clear_list_func(&(info->env));
        _clear_list_func(&(info->history));
        _clear_list_func(&(info->alias));

        free_func(info->environ);
        info->environ = NULL;
        mem_free_func((void **)info->cmd_buf);

        if (info->readfd > 2)
            close(info->readfd);

        my_putc_char(FL_BUFFER_CONST);
    }
}
