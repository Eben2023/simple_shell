#include "main.h"

/**
 * interac_mode - checks if program is running in interactive mode.
 * @info: pointer to a struct containing program information
 *
 * Return: 1 if running in interactive mode, 0 otherwise
 */

int interac_mode(dat_t *info)
{
    if (isatty(STDIN_FILENO) && info->readfd <= 2)
        return (1);
    else
        return (0);
}

/**
 * check_delim_ - function to check delimeter
 * @c: check
 * @delim: delimeter string pointer
 * Return: 1 when true, else 0
 */

int check_delim_(char c, char *delim)
{
    while (*delim)
        if (*delim++ == c)
            return (1);
    return (0);
}

/**
 * check_alpha - checks if a character is an alphabetic letter.
 * @c: character to check
 *
 * Return: 1 if c is an alphabetic letter, 0 otherwise.
 */

int check_alpha(int c)
{
    int is_alpha = 0;

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        is_alpha = 1;

    return (is_alpha);
}


/**
 * my_atoi_funct - converts a string to an integer.
 * @s: string to convert
 *
 * Return: converted integer
 */

int my_atoi_funct(char *s)
{
    int i = 0, sign = 1, output = 0;

    while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n')
        i++;

    if (s[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (s[i] == '+')
        i++;

    while (s[i] >= '0' && s[i] <= '9')
    {
        output = output * 10 + (s[i] - '0');
        i++;
    }
    return (output * sign);
}