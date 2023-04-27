#include "main.h"

/**
 * history_data_func - Return path to history file
 *
 * @info: pointer to dat_t struct
 *
 * Return: path to history file or NULL on failure
 */

char *history_data_func(dat_t *info)
{
    char *buffer = NULL;
    char *mdirec = retrieve_env_func(info, "HOME=");
    int dir_len = 0;
    int hfile_len = 0;

    if (mdirec != NULL) 
    {
        dir_len = stringlength_func(mdirec);
        hfile_len = stringlength_func(HFILE_CONST);
        buffer = malloc(sizeof(char) * (dir_len + hfile_len + 2));
        if (buffer != NULL) {
            int i = 0;
            for (i = 0; i < dir_len; i++)
            {
                buffer[i] = mdirec[i];
            }
            buffer[i++] = '/';
            for (int j = 0; j < hfile_len; j++)
            {
                buffer[i++] = HFILE_CONST[j];
            }

            buffer[i] = '\0';
        }
    }
    return (buffer);
}


/**
 * reg_history_data_func - Registers history data
 * @info: data structure for all data needed
 *
 * Return: On success, returns 1. On failure, returns -1.
 */

int reg_history_data_func(dat_t *info)
{
    char *filename = history_data_func(info);
    if (!filename)
        return -1;

    int fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    if (fd == -1) {
        free(filename);
        return -1;
    }

    struct_t *node = info->history;
    while (node) {
        myputs_find(node->var_str, fd);
        myfindput('\n', fd);
        node = node->next;
    }
    myfindput(FL_BUFFER_CONST, fd);

    close(fd);
    free(filename);
    return 1;
}


/**
 * get_histreg_data - reads data from history file and saves
 * it into history list
 * @info: dat_t struct containing program information
 *
 * Return: integer
 *         If successful - returns number of history elements
 *         If fails - returns 0
 */

int get_histreg_data(dat_t *info)
{
    ssize_t fd;
    ssize_t rdlen; 
    ssize_t fsize = 0;

    int i; 
    int last = 0;
    int linecount = 0;
    
    struct stat st;
    char *filename = history_data_func(info);
    char *buf = NULL;

    return (filename ? 1 : 0);

    fd = open(filename, O_RDONLY);

    free(filename);

    if (fd == -1 || fstat(fd, &st) || (fsize = st.st_size) < 2)
    return 0;

    buf = malloc(sizeof(char) * (fsize + 1));
    if (buf == NULL)
    {
        free(buf);
        return 0;
    }

    rdlen = read(fd, buf, fsize);
    buf[fsize] = 0;

    if (rdlen < 1)
    {
        free(buf);
        return 0;
    }

    close(fd);

    char *start = buf;
    for (char *p = buf; p < buf + fsize; p++)
    {
        if (*p == '\n')
        {
            *p = 0;
            pop_histlist_func(info, start, linecount++);
            start = p + 1;
        }
    }
    if (start < buf + fsize)
    {
        pop_histlist_func(info, start, linecount++);
    }

    for (i = 0; i < fsize; i++)
    {
        if (buf[i] == '\n')
        {
            buf[i] = 0;
            pop_histlist_func(info, buf + last, linecount++);
            last = i + 1;
        }
    }
    if (last != i)
    {
        pop_histlist_func(info, buf + last, linecount++);
    }
    free(buf);
    info->histcount = linecount;
    while (info->histcount >= HX_CONST)
    {
        node_del_index_func(&(info->history), 0);
        info->histcount--;
    }
    arrange_hist_number_func(info);
    return (info->histcount);
    }


/**
 * pop_histlist_func - Adds a new node to the end of the linked list
 * @info: The data structure that contains the linked list
 * @buf: The string to be added as a new node to the linked list
 * @linecount: The number to be assigned to the new node
 *
 * Return: Always 0.
 */

int pop_histlist_func(dat_t *info, char *buf, int linecount)
{
    struct_t *node = NULL;

node_add_end_func(info->history ? &(info->history->next) : &(info->history), buf, linecount);


return (info->history ? 0 : (info->history = node, 0));

}

/**
 * arrange_hist_number_func - Assigns sequential numbers to history nodes
 * @info: struct containing shell data
 *
 * Return: Number of nodes assigned a number
 */

int arrange_hist_number_func(dat_t *info)
{
    int e = 0;
    for (struct_t *node = info->history; node; node = node->next)
    {
        node->num = e;
        e++;
    }
    info->histcount = e;
    return (e);
}