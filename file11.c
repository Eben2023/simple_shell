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
int dir_len = 0, j;
int hfile_len = 0;

if (mdirec != NULL)
{
dir_len = stringlength_func(mdirec);
hfile_len = stringlength_func(HFILE_CONST);
buffer = malloc(sizeof(char) * (dir_len + hfile_len + 2));
if (buffer != NULL)
{
int i = 0;
for (i = 0; i < dir_len; i++)
{
buffer[i] = mdirec[i];
}
buffer[i++] = '/';
for (j = 0; j < hfile_len; j++)
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
struct_t *node;
char *filename = history_data_func(info);
int fd;
if (!filename)
return (-1);
fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
if (fd == -1)
{
free(filename);
return (-1);
}
node = info->history;
while (node)
{
myputs_find(node->var_str, fd);
myfindput('\n', fd);
node = node->next;
}
myfindput(FL_BUFFER_CONST, fd);

close(fd);
free(filename);
return (1);
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

node_add_end_func(info->history ? &(info->history->next) :
&(info->history), buf, linecount);

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
struct_t *node;
for (node = info->history; node; node = node->next)
{
node->num = e;
e++;
}
info->histcount = e;
return (e);
}
