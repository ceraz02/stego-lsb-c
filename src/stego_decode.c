#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <steg1ano/byte.h>
#include <steg1ano/contents.h>
#include <steg1ano/stego.h>

#define CANARY 0xefbeadde



int stego_decode_block( byte_t *ptr, size_t size, image_t img, int *p_row, int *p_col, int b ) {
  // TODO
  return -1;
}

int stego_decode_analyse_canary( image_t img ) {
  // TODO
  return -1;
}

int stego_decode( image_t img ) {
  // TODO
  return -1;
}
