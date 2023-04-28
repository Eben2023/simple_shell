#include "main.h"

/**
 * environ_var_func - display current environment
 * @info: Contains arguments
 * Return: Always 0
 */

int environ_var_func(dat_t *info)
{
list_str_display(info->env);
return (0);
}

/**
 * retrieve_env_func - Retrieve the value of an environment variable
 * @info: Pointer to dat_t struct
 * @name: Name of the environment variable
 *
 * Return: Pointer to the value of the environment variable if it exists,
 * NULL otherwise
 */

char *retrieve_env_func(dat_t *info, const char *name)
{
struct_t *node;
for (node = info->env; node != NULL; node = node->next)
{
char *p = begin_func(node->var_str, name);
if (p && *p)
return (p);
}
return (NULL);
}

/**
 * set_environ_func - Sets a value for an environment variable
 *
 * @info: Pointer to a dat_t struct containing program information
 *
 * Return: 0 on success, 1 on failure
 */

int set_environ_func(dat_t *info)
{
if (info->argc != 3)
{
myputs("Wrong required arguements\n");
return (1);
}

if (my_set_environ_func_(info, info->argv[1], info->argv[2]))
return (0);
return (1);
}

/**
 * unset_environ_func - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *constant function prototype.
 * Return: Always 0
 */

int unset_environ_func(dat_t *info)
{
int i = 0;

if (info->argc == 1)
{
myputs("Not enough arguements.\n");
return (1);
}
for (i = 1; i <= info->argc; i++)
my_unset_environ_func(info, info->argv[i]);

return (0);
}

/**
 * set_environ_data - fills env list
 * @info: Contains arguments.
 *
 * Return: Always 0
 */

int set_environ_data(dat_t *info)
{
size_t i;
struct_t *node = NULL;

for (i = 0; environ[i]; i++)
{
node_add_end_func(&node, environ[i], 0);
}

info->env = node;
return (0);
}
