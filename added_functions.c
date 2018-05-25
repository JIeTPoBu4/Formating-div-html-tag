#include <stdio.h>
#include <string.h>
#include "my_queue.h"

#define MAX_BUF_SIZE 256
#define MAX_ARRAY_SIZE 15
#define TERMINAL_STRING 80
#define SPACE_SIZE 4
#define DIV_SIZE 6
#define TERM_DIV_SIZE 7

char **read_from_stdin() {
	char input_buf[MAX_BUF_SIZE];
  char **buffer = NULL;

  if ((buffer = (char **)malloc(sizeof(char *) * MAX_ARRAY_SIZE)) == NULL) {
    printf("[error]");

    return 0;
  }

  buffer[0] = NULL;
  int count = 0;
  int max_size = MAX_ARRAY_SIZE;

  while (fgets(input_buf, MAX_BUF_SIZE, stdin)) {
    if (count == max_size) {
      char **add_buf = NULL;

      if ((add_buf = (char **)malloc(sizeof(char *) * max_size * 2)) == NULL) {
        for (int i = 0; i < count; i++) {
          free(buffer[i]);
        }

        free(buffer);
        printf("[error]");

        return NULL;
      }

      for (int i = 0; i < count; i++) {
        add_buf[i] = buffer[i];
      }

      free(buffer);
      buffer = add_buf;
      max_size *= 2;
    }

    if (strlen(input_buf)) {
      if (input_buf[strlen(input_buf) - 1] == '\n')
        input_buf[strlen(input_buf) - 1] = '\0';

      buffer[count] = (char *)malloc(sizeof(char) * (strlen(input_buf) + 1));

      if (!buffer[count]) {
        printf("[error]");
        free(buffer);

        return NULL;
      }

      snprintf(buffer[count], strlen(input_buf) + 1, "%s", input_buf);
    }

    count++;
  }

  if (count == max_size) {
    char **add_buf = NULL;

    if ((add_buf = (char **)malloc(sizeof(char *) * count + 1)) == NULL) {
      for (int i = 0; i < count; i++) {
        free(buffer[i]);
      }

      free(buffer);
      printf("[error]");

      return NULL;
    }

    for (int i = 0; i < count; i++) {
      add_buf[i] = buffer[i];
    }

    free(buffer);
    buffer = add_buf;
  }

  buffer[count] = NULL;

  return buffer;
}

int is_div(char *str) {

	if(*str == 'd') {
		char div_str[] = "div>";
		int i = 0;

		for( ; i < 4 && str[i]; i++) {
			if(str[i] != div_str[i])
				return 0;
		}

		if(!str[i] && i != 4)
			return 0;

		return 1;
	}
	else if(*str == '/') {
		char div_str[] = "/div>";
		int i = 0;

		for( ; i < 5 && str[i]; i++) {
			if(str[i] != div_str[i])
				return 0;
		}

		if(!str[i] && i != 5)
			return 0;

		return -1;
	}

	return 0;
}
/*
_Bool is_term_div(char *str) {
	char div_str[] = "/div>";

	int i = 0;

	for(int i = 0; i < 5 && str[i]; i++) {
		if(str[i] != div_str[i])
			return 0;
	}

	if(!str[i] && i != 5)
		return 0;

	return 1;
}
*/
char *alloc_str(size_t size) {
	char *str = NULL;

	if((str =(char*) malloc(size)) == NULL)
		return NULL;

	return str;
}

int put_div_queue(my_queue *queue, int open_div, int close_div) {
	char *buf_str = NULL;
  	char *space = NULL;

	if (!(buf_str = alloc_str(sizeof(char) * (DIV_SIZE + (open_div - close_div) * SPACE_SIZE)))) {
		printf("[error]");
		queue_clear(queue);

		return 0;
	}

	if (!(space = alloc_str(sizeof(char) * ((open_div - close_div) * SPACE_SIZE + 1)))) {
		printf("[error]");
		queue_clear(queue);

		return 0;
	}

	int iter = 0;

	for (; iter < (open_div - close_div) * SPACE_SIZE; iter++) {
		space[iter] = ' ';
	}

	space[iter] = '\0';
	snprintf(buf_str, DIV_SIZE + (open_div - close_div) * SPACE_SIZE, "%s<div>", space);

	if (queue_push(queue, buf_str)) {
		queue_clear(queue);
		free(buf_str);
		free(space);

		return 0;
	}

	free(buf_str);
	free(space);  

	return 1; 
}

int put_term_div_queue(my_queue *queue, int open_div, int close_div) {
	char *buf_str = NULL;
  	char *space = NULL;

	if (!(buf_str = alloc_str(sizeof(char) * (TERM_DIV_SIZE + (open_div - close_div) * SPACE_SIZE)))) {
		printf("[error]");
		queue_clear(queue);

		return 0;
	}

	if (!(space = alloc_str(sizeof(char) * ((open_div - close_div) * SPACE_SIZE + 1)))) {
		printf("[error]");
		free(buf_str);
		queue_clear(queue);

		return 0;
	}

	int iter = 0;

	for (; iter < (open_div - close_div) * SPACE_SIZE; iter++) {
		space[iter] = ' ';
	}

	space[iter] = '\0';
	snprintf(buf_str, TERM_DIV_SIZE + (open_div - close_div) * SPACE_SIZE, "%s</div>", space);

	if (queue_push(queue, buf_str)) {
		queue_clear(queue);
		free(buf_str);
		free(space);

		return 0;
	}

	free(buf_str);
	free(space);

	return 1;
}

int put_content_queue(const char *buffer, const int i, my_queue *queue, int *end_of_content, int open_div, int close_div) {
	int iter = i;
    char *buf_str = NULL;

    while (buffer[iter] != '<' && buffer[iter]) 
      iter++;

    *end_of_content = iter;

    if (buffer[iter - 1] == ' ') {
      iter -= 2;

      while (buffer[iter] == ' ') 
        iter--;
    } 
    else
      iter--;

    int size_of_str = iter - i + 2;

    if ((size_of_str + (open_div - close_div) * SPACE_SIZE - 1) <= TERMINAL_STRING) {
      if ((buf_str = (char *)malloc(sizeof(char) *(size_of_str + (open_div - close_div) * SPACE_SIZE))) == NULL) {
        printf("[error]");
        queue_clear(queue);

        return -1;
      }

      int j = 0;

      for (; j < (open_div - close_div) * SPACE_SIZE; j++) {
        buf_str[j] = ' ';
      }

      iter = i;

      for (; iter < i + size_of_str - 1; iter++) {
        buf_str[j] = buffer[iter];
        j++;
      }

      buf_str[j] = '\0';

      if (queue_push(queue, buf_str)) {
        queue_clear(queue);
        free(buf_str);

        return -1;
      }

      free(buf_str);
    } 
    else {
      int num_add_strings = (size_of_str - 1) / (TERMINAL_STRING - (open_div - close_div) * SPACE_SIZE);
      iter = i;
      int iterator = 1;

      for (; iterator <= num_add_strings; iterator++) {
        if ((buf_str = (char *)malloc(sizeof(char) *(TERMINAL_STRING + 1))) == NULL) {
          printf("[error]");
          queue_clear(queue);

          return -1;
        }

        int j = 0;

        for (; j < (open_div - close_div) * SPACE_SIZE; j++) {
          buf_str[j] = ' ';
        }

        for (; iter < i + (TERMINAL_STRING - (open_div - close_div) * SPACE_SIZE) * iterator; iter++) {
          buf_str[j] = buffer[iter];
          j++;
        }

        buf_str[j] = '\0';

        if (queue_push(queue, buf_str)) {
          queue_clear(queue);
          free(buf_str);

          return -1;
        }

        free(buf_str);
      }

      if ((size_of_str - 1) % (TERMINAL_STRING - (open_div - close_div) * SPACE_SIZE)) {
        int add_chr = (size_of_str - 1) % (TERMINAL_STRING - (open_div - close_div) * SPACE_SIZE);
        iterator--;

        if ((buf_str = (char *)malloc(sizeof(char) * (add_chr + (open_div - close_div) * SPACE_SIZE + 1))) == NULL) {
          printf("[error]");
          queue_clear(queue);

          return -1;
        }

        int j = 0;

        for (; j < (open_div - close_div) * SPACE_SIZE; j++) {
          buf_str[j] = ' ';
        }

        while (iter < i + (TERMINAL_STRING - (open_div - close_div) * SPACE_SIZE) * iterator + add_chr) {
          buf_str[j] = buffer[iter];
          j++;
          iter++;
        }

        buf_str[j] = '\0';

        if (queue_push(queue, buf_str)) {
          queue_clear(queue);
          free(buf_str);

          return -1;
        }

        free(buf_str);
      }
    }

    /*i = end_of_content - 1;
    continue;*/

    return 0;
}