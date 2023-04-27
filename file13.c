#include "main.h"

/**
 * disp_list_size - Counts the number of elements in a linked list.
 *
 * @h: Pointer to the head of the linked list.
 *
 * Return: Number of elements in the linked list.
 */

size_t disp_list_size(const struct_t *h)
{
size_t m = 0;

while (h)
{
h = h->next;
m = m + 1;
}
return (m);
}

/**
 * conv_list_to_str_func - function to return string arrays
 * @head: pointer
 *
 * Return: string array
 */

char **conv_list_to_str_func(struct_t *head)
{
char **_sstr;
char *var_str;

struct_t *nd = head;
size_t i = disp_list_size(head), j;

if (head == NULL || i == 0)
{
return (NULL);
}

_sstr = malloc(sizeof(char *) * (i + 1));
if (_sstr == NULL)
{
return (NULL);
}

for (i = 0; nd; nd = nd->next, i++)
{
var_str = malloc(stringlength_func(nd->var_str) + 1);
if (!var_str)
{
for (j = 0; j < i; j++)
free(_sstr[j]);
free(_sstr);

return (NULL);
}

var_str = mystring_cpy(var_str, nd->var_str);
_sstr[i] = var_str;
}
_sstr[i] = NULL;
return (_sstr);
}

/**
 * display_list_func_ - displays elements
 * @h: Points to the first node in the list
 *
 * Return: list size
 */

size_t display_list_func_(const struct_t *h)
{
size_t p = 0;

while (h)
{
my_puts_func_(my_num_conv_func(h->num, 10, 0));
my_putc_char(':');
my_putc_char(' ');
my_puts_func_(h->var_str ? h->var_str : "(nil)");
my_puts_func_("\n");
h = h->next;
p += 1;
}

return (p);
}

/**
 * _start_node_func - finds the first node in the linked list
 * that matches the prefix and character
 *
 * @node: a pointer to the first node in the linked list
 * @prefix: a pointer to the prefix string to match against the
 * variable strings
 * @c: a character to match against the first character of the
 * variable strings
 *
 * Return: a pointer to the first node in the linked list that
 * matches the prefix and character, or NULL if no match is found
 */

struct_t *_start_node_func(struct_t *node, char *prefix, char c)
{
char *str_start = NULL;

while (node != NULL)
{
str_start = begin_func(node->var_str, prefix);
if (str_start != NULL && (c == -1 || *str_start == c))
{
return (node);
}
node = node->next;
}

return (NULL);
}

/**
 * retr_node_index_func_ - returns the index of a node in a linked list
 *
 * @head: a pointer to the first node in the linked list
 * @node: a pointer to the node whose index is to be found
 *
 * Return: the index of the node in the linked list, or -1 if the node is
 * not found
 */

ssize_t retr_node_index_func_(struct_t *head, struct_t *node)
{
size_t index = 0;

while (head != NULL)
{
if (head == node)
{
return (index);
}
index++;
head = head->next;
}

return (-1);
}
