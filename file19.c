#include "main.h"

/**
 * stringlength_func - returns the length of a string.
 * @s: the string to get the length of.
 *
 * Description: This function returns the length of the 
 * string pointed to by the parameter 's', excluding the 
 * terminating null byte. If 's' is NULL, the function returns 0.
 *
 * Return: This function returns the length of the string 's' 
 * as an integer.
 */

int stringlength_func(char *s)
{
    int len = 0;

    if (s == NULL)
        return (0);

    while (*s != '\0')
    {
        len++;
        s++;
    }

    return (len);
}


/**
 * my_str_cmp - compares two strings.
 * @s1: the first string to compare.
 * @s2: the second string to compare.
 *
 * Description: This function compares two strings lexicographically 
 * by comparing the ASCII values of the characters and returns a 
 * negative, positive or zero integer depending on the result.
 *
 * Return: This function returns an integer less than, equal to, or greater than 0 if the first string
 *         is found to be less than, equal to, or greater than the second string, respectively.
 */

int my_str_cmp(char *s1, char *s2)
{
    int i = 0;

    while (s1[i] != '\0' && s2[i] != '\0')
    {
        if (s1[i] != s2[i])
            return (s1[i] - s2[i]);

        i++;
    }

    if (s1[i] == s2[i])
        return (0);

    return (s1[i] < s2[i] ? -1 : 1);
}


/**
 * begin_func - Returns a pointer to the first occurrence of the 
 * substring needle in the string haystack, or NULL if the substring 
 * is not found.
 *
 * @haystack: the string to search in
 * @needle: the substring to search for
 *
 * Return: a pointer to the first occurrence of the substring needle 
 * in the string haystack, or NULL if the substring is not found.
 */

char *begin_func(const char *haystack, const char *needle)
{
    while (*needle)
    {
        if (*needle++ != *haystack++)
        {
            return (NULL);
        }
    }
    
    return ((char *)haystack);
}


/**
 * mystring_cat - function concatenates two given strings
 * @dest: final destination
 * @src: origin
 *
 * Return: points to destination buffer
 */

char *mystring_cat(char *dest, char *src)
{
    char *rptr = dest;

    while (*dest)
    {
        dest++;
    }
    while (*src)
    {
        *dest++ = *src++;
    }

    *dest = *src;

    return (rptr);
}