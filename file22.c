#include "main.h"

/**
 * check_chain_func_ - Check if the current command buffer
 * ends with a chain operator.
 * @info: The dat_t struct.
 * @buf: The buffer string to be checked.
 * @p: The current index in the buffer string.
 *
 * Return: 1 if the current command buffer ends with a chain
 * operator, 0 otherwise.
 */

int check_chain_func_(dat_t *info, char *buf, size_t *p)
{
size_t st_var = *p;
int ret = 0;

switch (buf[st_var])
{
case '|':
if (buf[st_var + 1] == '|')
{
buf[st_var] = 0;
st_var++;
info->cmd_buf_type = OR_CM;
ret = 1;
}
break;
case '&':
if (buf[st_var + 1] == '&')
{
buf[st_var] = 0;
st_var++;
info->cmd_buf_type = AND_CM;
ret = 1;
}
break;
case ';':
buf[st_var] = 0;
info->cmd_buf_type = CH_CM;
ret = 1;
break;
}

*p = st_var;
return (ret);
}

/**
 * verify_chain_func_ - Updates the buffer and the pointer
 * @info: A pointer to a dat_t struct
 * @buf: A pointer to a buffer
 * @p: A pointer to a size_t
 * @i: A size_t representing the current index of the buffer
 * @len: A size_t representing the length of the buffer
 *
 * This function updates the buffer and pointer based on the status
 * of the cmd_buf_type of the dat_t struct. If the cmd_buf_type is
 * AND_CM and
 * status is true, the buffer is updated and the pointer is set to
 * the end of the buffer.
 * If the cmd_buf_type is OR_CM and status is false, the buffer is
 * updated
 * and the pointer is set to the end of the buffer.
 *
 * Return: void
 */

void verify_chain_func_(dat_t *info, char *buf, size_t *p,
size_t i, size_t len)
{
size_t svar = *p;

if (info->cmd_buf_type == AND_CM && info->status)
{
buf[i] = 0;
svar = len;
}
else if (info->cmd_buf_type == OR_CM && !info->status)
{
buf[i] = 0;
svar = len;
}

*p = svar;
}

/**
 * checkout_alias_func - Extracts the value from an alias name-value pair
 * @info: pointer to a struct containing program information
 *
 * Return: 1 if successful, 0 otherwise
 */

int checkout_alias_func(dat_t *info)
{
char *v_ptr;
struct_t *node;
int var_count;

for (var_count = 0; var_count < 10; var_count++)
{
node = _start_node_func(info->alias, info->argv[0], '=');

if (!node)
{
return (0);
}
free(info->argv[0]);

v_ptr = my_str_chr_func(node->var_str, '=');

if (!v_ptr)
{
return (0);
}

v_ptr = mystring_dup(v_ptr + 1);

if (!v_ptr)
{
return (0);
}

info->argv[0] = v_ptr;
}

return (1);
}

/**
 * checkout_variable_func_ - Replaces variables in the tokenized string
 * @info: The parameter struct containing the necessary information
 *
 * This function replaces variables in the tokenized string.
 * It replaces $?
 * with the exit status of the most recently executed command,
 * replaces $$ with
 * the process ID of the shell, and replaces environment variables
 * with their values.
 *
 * Return: 1 if variable was replaced, 0 otherwise.
 */

int checkout_variable_func_(dat_t *info)
{
struct_t *node;
int pos_val = 0;

for (pos_val = 0; info->argv[pos_val]; pos_val++)
{
if (info->argv[pos_val][0] != '$' || !info->argv[pos_val][1])
continue;

if (!my_str_cmp(info->argv[pos_val], "$?"))
{
checkout_str_func_(&(info->argv[pos_val]),
mystring_dup(my_num_conv_func(info->status, 10, 0)));
continue;
}
if (!my_str_cmp(info->argv[pos_val], "$$"))
{
checkout_str_func_(&(info->argv[pos_val]),
mystring_dup(my_num_conv_func(getpid(), 10, 0)));
continue;
}
node = _start_node_func(info->env, &info->argv[pos_val][1], '=');
if (node)
{
checkout_str_func_(&(info->argv[pos_val]),
mystring_dup(my_str_chr_func(node->var_str, '=') + 1));
continue;
}
checkout_str_func_(&info->argv[pos_val], mystring_dup(""));
}

return (0);
}

/**
 * checkout_str_func_ - Replaces a string with a new string
 * @old: The old string to be replaced
 * @new: The new string to replace the old string with
 *
 * This function replaces the old string with the new string,
 * by freeing the old
 * string and assigning the new string to the pointer of the old string.
 *
 * Return: 1 upon success, 0 upon failure.
 */

int checkout_str_func_(char **old, char *new)
{
free(*old);
*old = new;
return (1);
}
