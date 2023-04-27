#include "main.h"

/**
 * my_str_tw_func - splits a string into words based on a delimiter
 * @var_str: pointer to string to be split
 * @d: pointer to delimiter character(s)
 *
 * Return: pointer to an array of pointers to strings
 */

char **my_str_tw_func(char *var_str, char *d)
{
    char **ptr;

    int e;
    int f;
    int g;
    int h;
    int wordtally = 0;
    
    if (var_str == NULL || var_str[0] == 0)
    {
        return (NULL);
    }
    if (!d)
        d = " ";

    for (e = 0; var_str[e] != '\0'; e++)
        if (!check_delim_(var_str[e], d) && 
        (check_delim_(var_str[e + 1], d) || !var_str[e + 1]))
            wordtally++;

    if (wordtally == 0)
        {
            return (NULL);
        }

    ptr = malloc((1 + wordtally) * sizeof(char *));

    if (!ptr)
        {
            return (NULL);
        }

    for (e = 0, f = 0; f < wordtally; f++)
    {
        while (check_delim_(var_str[e], d))
            e++;
        g = 0;
        while (!check_delim_(var_str[e + g], d) && var_str[e + g])
            g++;
        ptr[f] = malloc((g + 1) * sizeof(char));
        if (!ptr[f])
        {
            for (g = 0; g < f; g++)
                free(ptr[g]);
            free(ptr);
            return (NULL);
        }
        for (h = 0; h < g; h++)
            ptr[f][h] = var_str[e++];
        ptr[f][h] = 0;
    }
    ptr[f] = NULL;
    return (ptr);
}


/**
 * my_strtws_func - splits a string into words using a given delimiter
 *
 * @var_str: string to split into words
 * @d: delimiter to use to split the string
 *
 * Return: array of strings (words) or NULL if var_str is NULL, empty or memory allocation fails
 */

char **my_strtws_func(char *var_str, char d)
{
    char **tptr;

    int xt;
    int ab;
    int tk;
    int tm;
    int twords = 0;
    
    if (var_str == NULL || var_str[0] == 0)
    {
        return (NULL);
    }

    for (xt = 0; var_str[xt] != '\0'; xt++)
        if ((var_str[xt] != d && var_str[xt + 1] == d) ||
            (var_str[xt] != d && !var_str[xt + 1]) || var_str[xt + 1] == d)
            twords++;

    if (twords == 0)
    {
        return (NULL);
    }

    tptr = malloc((1 + twords) * sizeof(char *));

    if (!tptr)
    {
        return (NULL);
    }

    for (xt = 0, ab = 0; ab < twords; ab++)
    {
        while (var_str[xt] == d && var_str[xt] != d)
            xt++;
        tk = 0;
        while (var_str[xt + tk] != d && var_str[xt + tk] && var_str[xt + tk] != d)
            tk++;
        tptr[ab] = malloc((tk + 1) * sizeof(char));
        if (!tptr[ab])
        {
            for (tk = 0; tk < ab; tk++)
                free(tptr[tk]);
            free(tptr);
            return (NULL);
        }
        for (tm = 0; tm < tk; tm++)
        {
            tptr[ab][tm] = var_str[xt++];
        }

        tptr[ab][tm] = 0;
    }
    tptr[ab] = NULL;
    
    return (tptr);
}