#include "main.h"

/**
 * get_histreg_data - retrieves and processes data from history file
 * @info: input data
 *
 * Return: 1 on success, 0 on failure
 */
int get_histreg_data(dat_t *info)
{
char *filename;
ssize_t fd;
ssize_t fsize;
char *buf;
struct stat st;
filename = history_data_func(info);
if (!filename)
return (0);

fd = open(filename, O_RDONLY);
free(filename);
if (fd == -1)
return (0);

if (fstat(fd, &st) == -1)
{
close(fd);
return (0);
}
fsize = st.st_size;
if (fsize < 2)
{
close(fd);
return (0);
}

buf = read_hist_data(fd, fsize);
close(fd);
if (!buf)
return (0);
process_hist_data(info, buf, fsize);
free(buf);
while (info->histcount >= HX_CONST)
{
node_del_index_func(&(info->history), 0);
info->histcount--;
}
arrange_hist_number_func(info);
return (info->histcount);
}

/**
 * read_hist_data - reads history data from file descriptor and
 * returns a buffer
 * @fd: file descriptor
 * @fsize: size of the file
 *
 * Return: pointer to the buffer on success, NULL on failure
 */
char *read_hist_data(ssize_t fd, ssize_t fsize)
{
char *buf;
ssize_t rdlen;

buf = malloc(sizeof(char) * (fsize + 1));
if (!buf)
return (NULL);

rdlen = read(fd, buf, fsize);
if (rdlen < 1)
{
free(buf);
return (NULL);
}

buf[fsize] = 0;
return (buf);
}

/**
 * process_hist_data - processes history data from buffer and populates
 * history list
 * @info: input data
 * @buf: history data buffer
 * @fsize: size of the buffer
 *
 * Return: void
 */
void process_hist_data(dat_t *info, char *buf, ssize_t fsize)
{
int i;
int last = 0;
int linecount = 0;
char *start;
char *p;
start = buf;
for (p = buf; p < buf + fsize; p++)
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
info->histcount = linecount;
}
