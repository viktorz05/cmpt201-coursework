#define _DEFAULT_SOURCE
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 128
#define HEAP_SIZE 256
#define BLOCK_SIZE 128

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(const char *err) {
  perror(err);
  exit(1);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data
                                                       : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

void print_block(struct header *block) {

  size_t data_size = block->size - sizeof(struct header);
  unsigned char *data = (unsigned char *)(block + 1);
  for (size_t i = 0; i < data_size; i++) {
    print_out("%u\n", data + i, sizeof(uint64_t));
  }
}

void *increase_heap_size(__intptr_t size) {

  void *address = sbrk(size);
  if (address == (void *)-1) {
    perror("sbrk() failed\n");
    return NULL;
  }
  return address;
}

void *initialize_block(void *block_ptr, uint64_t size, struct header *next,
                       int fill) {

  struct header *block = (struct header *)block_ptr;
  block->size = size;
  block->next = next;
  memset(block + 1, fill, size - sizeof(struct header));
  return block;
}
int main(void) {

  increase_heap_size(HEAP_SIZE);
  void *start_addr = sbrk(0);
  if (!start_addr)
    handle_error("sbrk()");

  void *snd_addr = (char *)start_addr + BLOCK_SIZE;

  struct header *fst_block = (struct header *)start_addr;
  struct header *snd_block = (struct header *)snd_addr;

  print_out("first block: \t%p\n", &fst_block, sizeof(void *));
  print_out("second block: \t%p\n", &snd_block, sizeof(void *));

  initialize_block(fst_block, BLOCK_SIZE, NULL, 0);
  initialize_block(snd_block, BLOCK_SIZE, fst_block, 1);

  print_out("first block size: %lu\n", &fst_block->size, sizeof(uint64_t));
  print_out("first block next: %p\n", &fst_block->next, sizeof(fst_block));
  print_out("second block size: %lu\n", &snd_block->size, sizeof(uint64_t));
  print_out("second block next: %p\n", &snd_block->next, sizeof(snd_block));
  print_block(fst_block);
  print_block(snd_block);
  return 0;
}
