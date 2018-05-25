#include "div_format.h"
#include "my_queue.h"
#include "added_functions.h"

#define TERMINAL_STRING 80
#define SPACE_SIZE 4
#define DIV_SIZE 6
#define TERM_DIV_SIZE 7

char **div_format(char **buffer) {
  int open_div = 0;
  int close_div = 0;
  my_queue queue;
  queue_init(&queue);

  for (int t = 0; buffer[t]; t++) {
    for (int i = 0; buffer[t][i]; i++) {

      if (open_div - close_div < 0) {
        printf("[error]");
        queue_clear(&queue);

        return NULL;
      }

      if (buffer[t][i] == '<') {

        switch(is_div(&buffer[t][i+1])) {
          case 1:
            if(!(put_div_queue(&queue, open_div, close_div))) 
            return NULL;

            open_div++;
            i += 4;
            continue;
          case -1:
            close_div++;

            if (open_div - close_div < 0) {
              printf("[error]");
              queue_clear(&queue);

              return NULL;
            }

            if(!put_term_div_queue(&queue, open_div, close_div)) 
              return NULL;

            i += 5;
            continue;
          case 0:
            printf("[error]");
            queue_clear(&queue);

            return NULL;
        }
      }

      if ((open_div - close_div >= 0) && (buffer[t][i] != ' ')) {
        int end_of_content = i;

        if(put_content_queue(buffer[t], i, &queue, &end_of_content, open_div, close_div)) {
            queue_clear(&queue);

            return NULL;
        }

        i = end_of_content - 1;
        continue;
      }
    }
  }

  if (open_div != close_div) {
    printf("[error]");
    queue_clear(&queue);

    return NULL;
  }

  char **array_of_str = NULL;
  int num_of_strings = queue_getsize(&queue);

  if ((array_of_str = (char **)malloc(sizeof(char *) * (num_of_strings + 1))) == NULL) {
    printf("[error]");
    queue_clear(&queue);

    return NULL;
  }

  for (int i = 0; i < num_of_strings; i++) {
    if (queue_pop(&queue, &array_of_str[i])) {
      printf("[error]");
      queue_clear(&queue);

      for (int z = 0; z < i; z++) {
        free(array_of_str[z]);
      }

      free(array_of_str);

      return NULL;
    }
  }

  array_of_str[num_of_strings] = NULL;

  return array_of_str;
}