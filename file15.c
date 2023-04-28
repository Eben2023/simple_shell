#include "main.h"

/**
 * mem_free_func - free memory pointed to by ptr and set it to NULL
 * @ptr: pointer to pointer to memory to free
 *
 * Return: 1 if memory was freed, 0 if ptr was NULL or already NULL
 */
int mem_free_func(void **ptr)
{
return ((ptr && *ptr) ? (free(*ptr), *ptr = NULL, 1) : 0);
}
