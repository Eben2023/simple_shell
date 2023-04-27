#include "main.h"

/**
 * check_cm - Check if file at path is a regular file
 * @info: unused
 * @path: path to file to check
 *
 * Return: 1 if file at path is a regular file, 0 otherwise
 */

int check_cm(dat_t *info, char *path)
{
    struct stat st;

    (void)info;

    if (!path || stat(path, &st))
    {
        return (0);
    }

    return ((st.st_mode & S_IFREG) == S_IFREG);

    return (0);
}

/**
 * chardup_func - Create a duplicate string from a substring of pathstr
 * @pathstr: the string to copy from
 * @start: the starting index of the substring
 * @stop: the index of the last character in the substring + 1
 *
 * Return: a duplicate string of the specified substring
 */

char *chardup_func(char *pathstr, int start, int stop)
{
    int x = 0;
    int y = 0;

    static char loc_buffer[1024];

    for (y = 0, x = start; x < stop; x++)
    {
        if (pathstr[x] != ':')
        {
            loc_buffer[y++] = pathstr[x];
        }
    }
    
    loc_buffer[y] = 0;

    return (loc_buffer);
}

/**
 * path_search - searches for a command in the directories of the PATH
 * @info: pointer to struct containing information about the shell
 * @pathstr: string of the PATH directories
 * @cmd: command to search for
 *
 * Return: pointer to the full path of the command if found, NULL otherwise
 */

char *path_search(dat_t *info, char *pathstr, char *cmd)
{
    char *path;

    int cpos_ = 0;
    int k = 0;

    if (!pathstr)
        return (NULL);
    if ((stringlength_func(cmd) > 2) && begin_func(cmd, "./"))
    {
        if (check_cm(info, cmd))
            return (cmd);
    }
    while (1)
    {
        if (!pathstr[k] || pathstr[k] == ':')
        {
            path = chardup_func(pathstr, cpos_, k);

            if (!*path)
            {
                mystring_cat(path, cmd);
            }
            else
            {
                mystring_cat(path, "/");
                mystring_cat(path, cmd);
            }
            if (check_cm(info, path))
                {
                    return (path);
                }
            if (!pathstr[k])
                {
                    break;
                }

            cpos_ = k;
        }
        k += 1;
    }
    return (NULL);
}