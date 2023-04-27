#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#define RD_BUFFER_CONST 1024
#define WR_BUFFER_CONST 1024
#define FL_BUFFER_CONST -1

#define NOM_CM 0
#define OR_CM 1
#define AND_CM 2
#define CH_CM 3

#define LOWCASE_CONST 1
#define UNSIGNED_CONST 2

#define GTLINE_CONST 0
#define STRTOK_CONST 0

#define HFILE_CONST ".epshell_history"
#define HX_CONST 4096

extern char **environ;

/**
 * struct string_list - linked list
 * @num: number
 * @var_str: string variable
 * @next: pointer that points to the next node on the list
 */

typedef struct string_list
{
    int num;
    char *var_str;
    struct string_list *next;
} struct_t;

/**
 * struct data_struct - contains arguments to pass into a function, allowing
 * uniform prototype for function pointer struct
 *
 * @arg: a string generated from the standard input stream containing the arguments
 * @argv: an array of strings generated from @arg
 * @path: a string representing the path to the current command
 * @argc: an integer representing the number of arguments passed to the command
 * @line_count: an integer representing the count of errors encountered
 * @err_num: an integer representing the error code for exit()
 * @linecount_flag: a boolean flag indicating whether the line count is on or off
 * @fname: a string representing the filename of the program
 * @env: a linked list containing a local copy of the environment variables
 * @environ: a custom modified copy of the environment variables from @env
 * @history: a node representing the command history
 * @alias: a node representing the command aliases
 * @env_changed: a boolean flag indicating whether the environment variables were changed
 * @status: an integer representing the return status of the last executed command
 * @cmd_buf: the address of a pointer to cmd_buf, indicating whether command chaining is on
 * @cmd_buf_type: an integer representing the type of command chaining (CMD_type ||, &&, ;)
 * @readfd: an integer representing the file descriptor from which to read the input line
 * @histcount: an integer representing the count of command history lines
 */

typedef struct data_struct
{
    char *arg;
    char **argv;
    char *path;
    int argc;
    unsigned int line_count;
    int err_num;
    int linecount_flag;
    char *fname;
    struct_t *env;
    struct_t *history;
    struct_t *alias;
    char **environ;
    int env_changed;
    int status;

    char **cmd_buf;
    int cmd_buf_type;
    int readfd;
    int histcount;
} dat_t;

/*define and initialize data*/
#define INITIALIZE_DATA                                                         \
    {                                                                           \
        NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
            0, 0, 0                                                             \
    }

/**
 * struct built_in_struct - contains functions
 * @bin_type: Command flag
 * @bin_func: Function
 */

typedef struct built_in_struct
{
    char *bin_type;
    int (*bin_func)(dat_t *);
} bin_struct;

int epshell(dat_t *, char **);
int built_in_look(dat_t *);
void cm_look(dat_t *);
void fork_func_look(dat_t *);

int check_cm(dat_t *, char *);
char *chardup_func(char *, int, int);
char *path_search(dat_t *, char *, char *);

int epshiterate(char **);

void myputs(char *);
int myputschar(char);
int myfindput(char c, int fd);
int myputs_find(char *var_str, int fd);

int stringlength_func(char *);
int my_str_cmp(char *, char *);
char *begin_func(const char *, const char *);
char *mystring_cat(char *, char *);

char *mystring_cpy(char *, char *);
char *mystring_dup(const char *);
void my_puts_func_(char *);
int my_putc_char(char);

char *mystring_cpy_funct(char *, char *, int);
char *my_string_cat_func(char *, char *, int);
char *my_str_chr_func(char *, char);

char **my_str_tw_func(char *, char *);
char **my_strtws_func(char *, char);

char *setmemb_func(char *, char, unsigned int);
void free_func(char **);
void *my_realloc_func(void *, unsigned int, unsigned int);

int mem_free_func(void **);

int interac_mode(dat_t *);
int check_delim_(char, char *);
int check_alpha_(int);
int my_atoi_funct(char *);

int my_atoi_alt(char *);
void disp_err_msg(dat_t *, char *);
int display_func_(int, int);
char *my_num_conv_func(long int, int, int);
void my_comment_remov(char *);

int term_exit(dat_t *);
int cd_impl_func(dat_t *);
int help_me_func(dat_t *);

int my_hist_func(dat_t *);
int my_alias_funct(dat_t *);

ssize_t my_read_input_func(dat_t *);
int _my_getline_func(dat_t *, char **, size_t *);
void my_signal_handler_func(int);

void del_data_func(dat_t *);
void reg_data_func(dat_t *, char **);
void release_data_func(dat_t *, int);

char *retrieve_env_func(dat_t *, const char *);
int environ_var_func(dat_t *);
int set_environ_func(dat_t *);
int unset_environ_func(dat_t *);
int set_environ_data(dat_t *);

char **my_retr_environ_func(dat_t *);
int my_unset_environ_func(dat_t *, char *);
int my_set_environ_func_(dat_t *, char *, char *);

char *history_data_func(dat_t *info);
int reg_history_data_func(dat_t *info);
int get_histreg_data(dat_t *info);
int pop_histlist_func(dat_t *info, char *buf, int linecount);
int arrange_hist_number_func(dat_t *info);

struct_t *node_add_func(struct_t **, const char *, int);
struct_t *node_add_end_func(struct_t **, const char *, int);
size_t list_str_display(const struct_t *);
int node_del_index_func(struct_t **, unsigned int);
void _clear_list_func(struct_t **);

size_t disp_list_size(const struct_t *);
char **conv_list_to_str_func(struct_t *);
size_t display_list_func_(const struct_t *);
struct_t *_start_node_func(struct_t *, char *, char);
ssize_t retr_node_index_func_(struct_t *, struct_t *);

int check_chain_func_(dat_t *, char *, size_t *);
void verify_chain_func_(dat_t *, char *, size_t *, size_t, size_t);
int checkout_alias_func(dat_t *);
int checkout_variable_func_(dat_t *);
int checkout_str_func_(char **, char *);

#endif