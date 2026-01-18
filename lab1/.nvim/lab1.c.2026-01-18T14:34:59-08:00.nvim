#define _POSIX_C_SOURCE 200809L
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
  char *buf = NULL;
  size_t len = 0;
  ssize_t read;
  printf("Please enter some text: ");
  if ((read = getline(&buf, &len, stdin)) != -1) {
    printf("Tokens:\n");
    char *tok = NULL;
    char *saveptr = NULL;
    tok = strtok_r(buf, " ", &saveptr);
    while (tok != NULL) {
      printf("%s\n", tok);
      tok = strtok_r(NULL, " ", &saveptr);
    }
  } else {
    perror("getline failed\n");
    exit(EXIT_FAILURE);
  }
  free(buf);

  return 0;
}
