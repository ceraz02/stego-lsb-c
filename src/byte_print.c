#include <stdio.h>
#include <assert.h>

#include <steg1ano/byte.h>



void print_octets(byte_t *ptr, size_t size) {
  printf("ptr -> ");
  for(size_t i = 0 ; i < size ; i ++) {
      printf("[%lu]=0x%hhx ", i, ptr[i]);
  }
  printf("\n");
}
