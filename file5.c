#include "main.h"

/**
 * myputs - displays a string
 * @var_str: printed string
 *
 * Return: None
 */

void myputs(char *var_str)
{
    if (!var_str)
        return;
    for (; *var_str != '\0'; var_str++)
        myputschar(*var_str);
}

/**
 * myputschar - writes a single character to standard output
 * @c: the character to be written
 *
 * Return: On success 1.
 */

int myputschar(char c)
{
    static int i = 0;
    static char buf[WR_BUFFER_CONST] = {0};

    if (c == FL_BUFFER_CONST || i >= WR_BUFFER_CONST)
    {
        write(STDERR_FILENO, buf, i);
        i = 0;
    }
    if (c != FL_BUFFER_CONST)
        buf[i++] = c;

    return (1);
}

/**
 * myfindput - writes character to fd
 * @c: character print
 * @fd: filedescriptor
 *
 * Return: On success, 1. On failure, 0.
 * In case of an error, -1 is returned
 */

int myfindput(char c, int fd)
{
    static int count;
    static char buffer[WR_BUFFER_CONST];

    if (c == FL_BUFFER_CONST || count >= WR_BUFFER_CONST)
    {
        write(fd, buffer, count);
        count = 0;
    }
    if (c != FL_BUFFER_CONST)
        buffer[count++] = c;

    return (1);
}

/**
 * myputs_find - writes a string to a given file descriptor
 * @var_str: The string to write
 * @fd: The file descriptor to write to
 *
 * Return: The number of characters written
 */

int myputs_find(char *var_str, int fd)
{
    int i = 0;

    if (var_str == NULL)
        return (0);

    while (var_str[i] != '\0')
    {
        myfindput(var_str[i], fd);
        i++;
    }

    return (i);
}