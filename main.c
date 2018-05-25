#include "div_format.h"
#include "my_queue.h"
#include "added_functions.h"

int main(int argc, char *argv[]) {
  char ** buffer = NULL;

  if((buffer = read_from_stdin()) == NULL) 
    return 0; 

  char **array_of_str = div_format(buffer);

  if (array_of_str) {
    for (int i = 0; array_of_str[i]; i++) {
      printf("%s\n", array_of_str[i]);
      free(array_of_str[i]);
    }

    free(array_of_str);
  }

  for (int i = 0; buffer[i]; i++) {
    free(buffer[i]);
  }

  free(buffer);

  return 0;
}