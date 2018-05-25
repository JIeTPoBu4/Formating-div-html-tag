#ifndef _ADDED_FUNCTIONS
#define _ADDED_FUNCTIONS

char **read_from_stdin();
int is_div(char *str);
//_Bool is_term_div(char *str);
char *alloc_str(size_t size);
int put_div_queue(my_queue *queue, int open_div, int close_div);
int put_term_div_queue(my_queue *queue, int open_div, int close_div);
int put_content_queue(const char *buffer, const int i, my_queue *queue, 
	int *end_of_content, int open_div, int close_div);

#endif