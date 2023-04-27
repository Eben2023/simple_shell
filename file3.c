#include "main.h"

/**
 * my_hist_func - prints the command history
 * @info: input command and arguments
 *
 * Return: Always 0.
 */

int my_hist_func(dat_t *info)
{
    display_list_func_(info->history);
    return (0);
}

/**
 * unset_alias - remove an alias from the list of aliases
 * @info: pointer to struct containing command line information
 * @var_str: string representing the alias to be removed
 *
 * Return: 1 on failure, 0 on success
 */

int unset_alias(dat_t *info, char *var_str)
{
    int ret = 0;
    char *p;
    char c;

    p = my_str_chr_func(var_str, '=');
    if (!p)
        return (1);
    c = *p;
    *p = '\0';
    ret = node_del_index_func(&(info->alias),
        retr_node_index_func_(info->alias,
        _start_node_func(info->alias, var_str, -1)));

    *p = c;
    
    return (ret);
}

/**
 * set_alias - sets alias to string
 * @info: parameter struct
 * @var_str: the string alias
 *
 * Return: 0 on success, else 1
 */

int set_alias(dat_t *info, char *var_str)
{
    char *p;

    p = my_str_chr_func(var_str, '=');
    if (p == NULL)
        return (1);
    if (!*++p)
        return (unset_alias(info, var_str));

    unset_alias(info, var_str);
    return (node_add_end_func(&(info->alias), var_str, 0) ? 0 : 1);
}

/**
 * print_alias - Prints the variable and value of an alias node
 * @node: A pointer to the alias node to print
 *
 * Return: 0 on success, else 1
 */

int print_alias(struct_t *node)
{
    char *p = NULL, *a = NULL;

    if (!node)
        return (1);

    p = my_str_chr_func(node->var_str, '=');
    for (a = node->var_str; a <= p; a++)
        my_putc_char(*a);
    my_putc_char('\'');
    my_puts_func_(p + 1);
    my_puts_func_("'\n");

    return (0);
}

/**
 * my_alias_funct - alias built_in_struct
 * @info: contains arguments
 *  Return: Always 0
 */

int my_alias_funct(dat_t *info)
{
    struct_t *node = NULL;
    char *p = NULL;
    int i = 0;

    if (info->argc == 1)
    {
        node = info->alias;
        for (struct_t *curr = node; curr != NULL; curr = curr->next)
        {
            print_alias(curr);
        }

        return (0);
    }
    for (i = 1; info->argv[i]; i++)
    {
        p = my_str_chr_func(info->argv[i], '=');
        if (p)
            set_alias(info, info->argv[i]);
        else
            print_alias(_start_node_func(info->alias, info->argv[i], '='));
    }

    return (0);
}