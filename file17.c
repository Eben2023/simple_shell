#include "main.h"

/**
 * setmemb_func - sets the first n bytes of the memory area pointed to by s
 * to the constant byte b
 *
 * @s: pointer to the memory area to be set
 * @b: constant byte to set
 * @n: number of bytes to be set to b
 *
 * Return: pointer to the memory area s
 */

char *setmemb_func(char *s, char b, unsigned int n)
{
unsigned int x;

for (x = 0; x < n; x++)
s[x] = b;
return (s);
}

/**
 * free_func - frees all memory blocks pointed to by the double pointer pp
 *
 * @pp: double pointer to be freed
 *
 * Return: void
 */

void free_func(char **pp)
{
char **ptr = pp;

if (!pp)
return;

while (*ptr != NULL)
{
free(*ptr);
*ptr = NULL;
ptr++;
}

free(pp);
}

/**
 * my_realloc_func - Reallocates memory block
 * @ptr: Pointer to previously allocated memory block
 * @old_size: Size of old memory block
 * @new_size: Size of new memory block
 *
 * Return: Pointer to the new memory block, or NULL on failure
 */

void *my_realloc_func(void *ptr, unsigned int old_size, unsigned int new_size)
{
char *new_ptr, *old_ptr;
unsigned int i;

if (!ptr)
return (malloc(new_size));
if (!new_size)
{
free(ptr);
return (NULL);
}
if (new_size == old_size)
return (ptr);

new_ptr = malloc(new_size);
if (!new_ptr)
return (NULL);

old_ptr = ptr;
for (i = 0; i < old_size && i < new_size; i++)
new_ptr[i] = old_ptr[i];
free(ptr);
return (new_ptr);
}
