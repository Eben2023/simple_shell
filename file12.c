#include "main.h"

/**
 * node_add_func - adds a new node to the beginning of a linked list
 *
 * @head: pointer to the head of the linked list
 * @var_str: string to be stored in the new node
 * @num: integer to be stored in the new node
 *
 * Return: pointer to the new head of the linked list
 */

struct_t *node_add_func(struct_t **head, const char *var_str, int num)
{
struct_t *new_head;

if (head == NULL)
{
return (NULL);
}

new_head = malloc(sizeof(struct_t));
if (new_head == NULL)
{
return (NULL);
}

setmemb_func((void *)new_head, 0, sizeof(struct_t));

new_head->num = num;
if (var_str)
{
new_head->var_str = mystring_dup(var_str);
if (new_head->var_str == NULL)
{
free(new_head);
return (NULL);
}
}

new_head->next = *head;
*head = new_head;
return (new_head);
}

/**
 * node_add_end_func - appends node to list
 * @head: pointer to head node
 * @var_str: variable string of node
 * @num: index
 *
 * Return: list
 */

struct_t *node_add_end_func(struct_t **head, const char *var_str, int num)
{
struct_t *new_node, *node;

if (head == NULL)
{
return (NULL);
}

node = *head;
new_node = malloc(sizeof(struct_t));

if (new_node == NULL)
{
return (NULL);
}
setmemb_func((void *)new_node, 0, sizeof(struct_t));
new_node->num = num;
if (var_str)
{
new_node->var_str = mystring_dup(var_str);
if (new_node->var_str == NULL)
{
free(new_node);
return (NULL);
}
}
if (node)
{
while (node->next)
{
node = node->next;
}
node->next = new_node;
}
else
{
*head = new_node;
}
return (new_node);
}

/**
 * list_str_display - traverses a linked list and prints the value of
 * each node's var_str member
 *
 * @h: pointer to the head of the linked list
 *
 * Return: number of nodes in the linked list
 */

size_t list_str_display(const struct_t *h)
{
size_t u = 0;

/*Func: traverses a linked list and prints the value*/
while (h)
{
my_puts_func_(h->var_str ? h->var_str : "(nil)");
my_puts_func_("\n");
h = h->next;
u += 1;
}
return (u);
}

/**
 * node_del_index_func - deletes the node at a given index in a linked list
 *
 * @head: double pointer to the head of the linked list
 * @index: index of the node to be deleted
 *
 * Return: 1 if the node is successfully deleted, 0 otherwise
 */

int node_del_index_func(struct_t **head, unsigned int index)
{
unsigned int a = 0;
struct_t *prev_node;
struct_t *node;

if (!head || !*head)
{
return (0);
}

if (index == 0)
{
node = *head;
*head = (*head)->next;
free(node->var_str);
free(node);
return (1);
}

node = *head;

while (node)
{
if (a == index)
{
prev_node->next = node->next;
free(node->var_str);
free(node);
return (1);
}
a += 1;

prev_node = node;
node = node->next;
}

return (0);
}

/**
 * _clear_list_func - Frees all memory associated with a linked list.
 *
 * @head_ptr: Pointer to a pointer to the head of the linked list.
 *
 * This function frees all memory associated with a linked list.
 * If the head of the list is NULL, the function simply returns.
 * Otherwise, the function iterates through the list and frees
 * each node and its associated memory, including the node's var_str.
 * Finally, the head of the list is set to NULL.
 */

void _clear_list_func(struct_t **head_ptr)
{
struct_t *nd, *next_nd;

if (head_ptr == NULL || *head_ptr == NULL)
{
return;
}

nd = *head_ptr;

while (nd)
{
next_nd = nd->next;
free(nd->var_str);
free(nd);
nd = next_nd;
}

*head_ptr = NULL;
}
