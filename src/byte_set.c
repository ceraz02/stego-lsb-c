#include <stdio.h>
#include <assert.h>

#include <steg1ano/byte.h>


int byte_set_lsb( byte_t *p_byte, unsigned int b, byte_t val ) {
  // TODO

  /* Vérification des paramètres */
  if ((b<1) || (b>8)) return 1;
  if (NULL == p_byte)  return 2;

  /* Modification du LSB */
  *p_byte = (*p_byte & (0xFF << b)) | (val & (0xFF >> (8-b)));

  return 0;
}

int byte_set_bits( byte_t * p_byte, unsigned int firstbit, unsigned int b, byte_t val ) {
  // TODO

  /* Vérification des paramètres */
  if ( (firstbit + b) > 8) return 1;
  if (NULL == p_byte)  return 2;

  /* Modification des bits */
  *p_byte = (*p_byte & ((0xFF << (firstbit+b)) | (0xFF >> (8-firstbit))))  | ((val & (0xFF >> (8 - b))) << firstbit);

  return 0;
}
