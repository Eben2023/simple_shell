#include "main.h"

/**
 * my_atoi_alt - converts a string to an integer
 * @s: The string to be converted
 *
 * Return: The converted integer value
 *         or -1 if s does not contain a valid integer
 */

int my_atoi_alt(char *s)
{
    int i = 0;
    unsigned long int result = 0;

    if (*s == '+')
        s++;

    for (i = 0; s[i] != '\0'; i++)
    {
        if (s[i] >= '0' && s[i] <= '9')
        {
            result *= 10;
            result += (s[i] - '0');
            if (result > INT_MAX)
                return (-1);
        }
        else
        {
            return (-1);
        }
    }
    return (result);
}


/**
 * disp_err_msg - Display error message with file, line and function name
 * @info: pointer to dat_t structure holding command info
 * @estr: string containing error message
 *
 * Return: void
 */

void disp_err_msg(dat_t *info, char *estr)
{
    myputs(info->fname);
    myputs(": ");
    display_func_(info->line_count, STDERR_FILENO);
    myputs(": ");
    myputs(info->argv[0]);
    myputs(": ");
    myputs(estr);
}

/**
 * display_func_ - function prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the filedescriptor to write to
 *
 * Return: number of characters printed
 */
int display_func_(int input, int fd)
{
    int (*__putchar)(char) = my_putc_char;
    int i, count = 0;
    unsigned int _abs_, current;

    if (fd == STDERR_FILENO)
        __putchar = myputschar;
    if (input < 0)
    {
        _abs_ = -input;
        __putchar('-');
        count++;
    }
    else
        _abs_ = input;
    current = _abs_;
    for (i = 1000000000; i > 1; i /= 10)
    {
        if (_abs_ / i)
        {
            __putchar('0' + current / i);
            count++;
        }
        current %= i;
    }
    __putchar('0' + current);
    count++;

    return (count);
}

/**
 * my_num_conv_func - converts a number to a string representation
 * @num: the number to convert
 * @base: the numerical base to use (2-16)
 * @flags: bitfield of conversion flags
 *
 * Return: a pointer to the converted string
 **/

char *my_num_conv_func(long int num, int base, int flags)
{
    static char buffer[50];
    char *ptr = &buffer[50 - 1];
    *ptr = '\0';

    if (!(flags & UNSIGNED_CONST) && num < 0)
    {
        num = -num;
        *--ptr = '-';
    }

    if (num == 0)
    {
        *--ptr = '0';
    }
    else
    {
        static char digits[] = "0123456789ABCDEF";
        while (num != 0)
        {
            *--ptr = digits[num % base];
            num /= base;
        }
    }

    return (ptr);
}


/**
 * my_comment_remov - replaces first '#' to '\0'
 * @buf: address
 *
 * Return: Always 0
 */

void my_comment_remov(char *buf)
{
    int i;

    for (i = 0; buf[i] != '\0'; i++)
    {
        if (buf[i] == '#' && (i == 0 || buf[i - 1] == ' '))
        {
            buf[i] = '\0';
            break;
        }
    }
}