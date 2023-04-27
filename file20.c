#include "main.h"

/**
 * mystring_cpy - copies the string pointed to by 'src'
 * into the buffer pointed to by 'dest'
 * @dest: destination buffer
 * @src: source string
 *
 * Return: pointer to 'dest'
 */

char *mystring_cpy(char *dest, char *src)
{
char *ret = dest;

while ((*dest++ = *src++))
;
return (ret);
}

/**
 * mystring_dup - duplicates a string
 *
 * @var_str: the string to be duplicated
 *
 * Return: returns a pointer to the duplicated string or NULL if
 * it fails.
 */

char *mystring_dup(const char *var_str)
{
int length = 0, i;
char *ret;

if (!var_str)
return (NULL);
while (var_str[length])
length++;
ret = malloc(sizeof(char) * (length + 1));
if (!ret)
return (NULL);
for (i = 0; i <= length; i++)
ret[i] = var_str[i];
return (ret);
}

/**
 * my_puts_func_ - prints a string to stdout followed by a newline
 * @var_str: pointer to a string to be printed
 *
 * Return: void
 */

void my_puts_func_(char *var_str)
{
if (!var_str)
return;
while (*var_str)
my_putc_char(*var_str++);
}

/**
 * my_putc_char - writes the character c to stdout
 * @c: the character to be written
 *
 * Return: 1 on success, -1 on failure
 */

int my_putc_char(char c)
{
static char loc_buffer[WR_BUFFER_CONST];
static int b;

if (c == FL_BUFFER_CONST || b >= WR_BUFFER_CONST)
{
if (write(1, loc_buffer, b) == -1)
{
return (-1);
}
b = 0;
}

if (c != FL_BUFFER_CONST)
{
loc_buffer[b++] = c;
}

return (1);
}
