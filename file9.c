#include "main.h"

/**
 * my_retr_environ_func - This function retrieves the environment variables as an array of strings.
 *
 * @info: A structure containing relevant parameters.
 *
 * Return: This function returns an array of strings that represent the current environment variables.
 */

char **my_retr_environ_func(dat_t *info)
{
    int _is_env_or_change = (!info->environ || info->env_changed);

    if (_is_env_or_change)
    {
        info->environ = conv_list_to_str_func(info->env);
        info->env_changed = 0;
    }

    return (info->environ);
}

/**
 * my_unset_environ_func - Removes an environment variable.
 * @info: Pointer to a struct containing environment variables.
 * @var: Variable to be removed.
 *
 * Return: 1 if successful, 0 otherwise.
 */

int my_unset_environ_func(dat_t *info, char *var)
{
    char *p;
    size_t i = 0; 
    struct_t *node = info->env;

    if (node == NULL || var == NULL)
    {
        return 0;
    }


    for (i = 0, node = info->env; node; i++, node = node->next)
    {

    p = begin_func(node->var_str, var);

    if (p && *p == '=')
        {
            info->env_changed = node_del_index_func(&(info->env), i);
            i = -1;
            node = info->env;
        }
    }

    return (info->env_changed);
}

/**
 * my_set_environ_func_ - set environment
 * @info: contains arguments
 * @var: string
 * @value: the string env var value
 * Return: (0)
 */

int my_set_environ_func_(dat_t *info, char *var, char *value)
{
    char *p, *buf = NULL;
    struct_t *node;
    int x = (!var || !value);
    if (x)
    {
        return (0);
    }

    buf = malloc((stringlength_func(var)) + (stringlength_func(value) + 2));
    
    if (buf == NULL)
    {
        return (1);
    }
    
    mystring_cpy(buf, var);
    mystring_cat(buf, "=");
    mystring_cat(buf, value);
    node = info->env;

    while (node)
    {
        p = begin_func(node->var_str, var);
        if (p && *p == '=')
        {
            free(node->var_str);
            node->var_str = buf;
            info->env_changed = 1;
            return (0);
        }
        node = node->next;
    }
    
    node_add_end_func(&(info->env), buf, 0);
    free(buf);
    info->env_changed = 1;
    return (0);
}