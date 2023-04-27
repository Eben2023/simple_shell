#include "main.h"

/**
 **mystring_cpy_funct - Copies a string
 *@dest: pointer to destination string
 *@src: pointer to source string
 *@n: maximum number of characters to copy
 *Return: a pointer to the destination string
 */

char *mystring_cpy_funct(char *dest, char *src, int n)
{
    int i;

    for (i = 0; i < n && src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }
    for (; i < n; i++)
    {
        dest[i] = '\0';
    }

    return (dest);
}


/**
 * my_string_cat_func - Concatenates two strings
 *
 * @dest: Destination string
 * @src: Source string
 * @n: Maximum number of characters to concatenate
 *
 * Return: Pointer to destination string
 */

char *my_string_cat_func(char *dest, char *src, int n)
{
int i, j;
char *s = dest;
i = 0;
while (dest[i] != '\0')
{
    i++;
}
for (j = 0; j < n && src[j] != '\0'; j++)
{
    dest[i + j] = src[j];
}
dest[i + j] = '\0';
return s;
}

/**
 * my_str_chr_func - search for the first occurrence of a character in a string
 *
 * @s: the string to be searched
 * @c: the character to search for
 *
 * Return: a pointer to the first occurrence of the character in the string, 
 * or NULL if the character is not found
 */
char *my_str_chr_func(char *s, char c)
{
    while (*s != '\0' && *s != c)
    {
        s++;
    }
    if (*s == c)
    {
        return (s);
    }
}