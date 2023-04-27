#include "main.h"

/**
 * input_buf - commands
 * @info: parameter struct
 * @buf: Para for buffer address
 * @len: Para for len var address
 *
 * Return: bytes read
 */

ssize_t input_buf(dat_t *info, char **buf, size_t *len)
{
ssize_t r = 0;
size_t len_p = 0;

if (*len == 0)
{
free(*buf);
*buf = NULL;
signal(SIGINT, my_signal_handler_func);

#if GTLINE_CONST
r = getline(buf, &len_p, stdin);
#else
r = _my_getline_func(info, buf, &len_p);
#endif
if (r > 0)
{
if ((*buf)[r - 1] == '\n')
{
(*buf)[r - 1] = '\0';
r--;
}

info->linecount_flag = 1;

my_comment_remov(*buf);

pop_histlist_func(info, *buf, info->histcount++);
{
*len = r;
info->cmd_buf = buf;
}
}
}
return (r);
}

/**
 * my_read_input_func - captures a new line minus another newline
 * @info: parameter
 *
 * Return: amount of bytes
 */

ssize_t my_read_input_func(dat_t *info)
{
static char *buf;
static size_t j;
static size_t i;
static size_t len;
ssize_t r = 0;
char *p;
char **buf_p = &(info->arg);
my_putc_char(FL_BUFFER_CONST);
r = input_buf(info, &buf, &len);
if (r == -1)
{
return (-1);
}
if (len)
{
j = i;
p = buf + i;
verify_chain_func_(info, buf, &j, i, len);
for (j = 0; j < len; j++)
{
if (check_chain_func_(info, buf, &j))
{
break;
}
}
i = j + 1;
if (i >= len)
{
i = len = 0;
info->cmd_buf_type = NOM_CM;
}
*buf_p = p;
return (stringlength_func(p));
}
*buf_p = buf;
return (r);
}

/**
 * read_buf - function for reading buffer
 * @info: The struct parameter
 * @buf: The buffer of type char
 * @i: The size of the para
 *
 * Return: (r) of type ssize_t
 */

ssize_t read_buf(dat_t *info, char *buf, size_t *i)
{
ssize_t p = 0;
ssize_t r = 0;

if (*i)
{
return (0);
}
p = read(info->readfd, buf, RD_BUFFER_CONST);
r = p;

if (r >= 0)
{
*i = r;
}

return (r);
}

/**
 * _my_getline_func - This function retrieves the next line of input
 * from the standard input (STDIN)
 * @info: A structure containing relevant parameters.
 * @ptr: The address of a pointer to a buffer, which may be preallocated
 * or set to NULL.
 * @length: The size of the preallocated ptr buffer, if it is not NULL
 *
 * Return: The function returns the next line of input as a string.
 */

int _my_getline_func(dat_t *info, char **ptr, size_t *length)
{
ssize_t r; /*added*/
size_t k, s = 0;
static char buf[RD_BUFFER_CONST];
static size_t len;
static size_t i;
char *new_p = NULL;
char *c;
char *p = NULL;
p = *ptr;
if (p && length)
s = *length;
if (i == len)
i = len = 0;
r = read_buf(info, buf, &len);
if ((r == -1) || (r == 0 && len == 0))
return (-1);
c = my_str_chr_func(buf + i, '\n');
k = c ? 1 + (unsigned int)(c - buf) : len;
new_p = my_realloc_func(p, s, s ? s + k : k + 1);
if (!new_p)
{
return (p ? free(p), -1 : -1);
}
if (s)
{
my_string_cat_func(new_p, buf + i, k - i);
}
else
{
mystring_cpy_funct(new_p, buf + i, k - i + 1);
}
s += k - i;
i = k;
p = new_p;
if (length)
*length = s;
*ptr = p;
return (s);
}

/**
 * my_signal_handler_func - This function blocks the ctrl-C signal.
 *
 * @sig_num: The signal number.
 *
 * Return: void
 */

void my_signal_handler_func(__attribute__((unused)) int sig_num)
{
my_puts_func_("\n");
my_puts_func_("$ ");
my_putc_char(FL_BUFFER_CONST);
}
