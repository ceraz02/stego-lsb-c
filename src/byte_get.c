#include <stdio.h>
#include <assert.h>

#include <steg1ano/byte.h>



int byte_get_lsb( byte_t byte, unsigned int b, byte_t *p_lsb ) {
  // TODO

  /* Vérification des paramètres */
  if ((b<1) || (b>8)) return 1;
  if (NULL == p_lsb)  return 2;
  
  /* Récupération du LSB */
  *p_lsb = byte & (0xFF >> (8-b));
  
  return 0;
}


int byte_get_bits( byte_t byte, unsigned int firstbit, unsigned int b, byte_t *p_res ) {
  // TODO

  /* Vérification des paramètres */
  if ( (firstbit + b) > 8) return 1;
  if (NULL == p_res)  return 2;

  /* Récupération des bits */
  *p_res = (byte >>(firstbit)) & (0xFF >> (8-b)); 

  return 0;
}