#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <steg1ano/byte.h>


void test_byte_get_lsb( void ) {
  byte_t val = 0;

  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : On peut lire les LSB d'un octet :\e[m\n" );
  assert( 0 == byte_get_lsb( 0xAC, 4, &val ) );
  assert( 0x0C == val );
  assert( 0 == byte_get_lsb( 0xAB, 4, &val ) );
  assert( 0x0B == val );
  assert( 0 == byte_get_lsb( 0xFA, 2, &val ) );
  assert( 0x02 == val );
  assert( 0 == byte_get_lsb( 0xFE, 1, &val ) );
  assert( 0x00 == val );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Lire plus de 8 bits est bien détecté comme une erreur :\e[m\n" );
  assert( 0 != byte_get_lsb( 0xaa, 9, &val ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Passer NULL en troisième paramètre est bien détecté comme une erreur :\e[m\n" );
  assert( 0 != byte_get_lsb( 0xaa, 2, NULL ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}

void test_byte_get_bits( void ) {
  byte_t extracted_bits = 0x00;

  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : On peut récupérer une série de bits dans octet :\e[m\n" );
  assert( 0 == byte_get_bits( 0xE1, 0, 1, &extracted_bits ) );
  assert( 0x01 == extracted_bits );

  assert( 0 == byte_get_bits( 0xE1, 4, 1, &extracted_bits ) );
  assert( 0x00 == extracted_bits );

  assert( 0 == byte_get_bits( 0xE1, 7, 1, &extracted_bits ) );
  assert( 0x01 == extracted_bits );

  assert( 0 == byte_get_bits( 0xE1, 0, 2, &extracted_bits ) );
  assert( 0x01 == extracted_bits );

  assert( 0 == byte_get_bits( 0xE1, 2, 2, &extracted_bits ) );
  assert( 0x00 == extracted_bits );

  assert( 0 == byte_get_bits( 0xE1, 4, 2, &extracted_bits ) );
  assert( 0x02 == extracted_bits );

  assert( 0 == byte_get_bits( 0xE1, 6, 2, &extracted_bits ) );
  assert( 0x03 == extracted_bits );

  assert( 0 == byte_get_bits( 0xE1, 0, 4, &extracted_bits ) );
  assert( 0x01 == extracted_bits );

  assert( 0 == byte_get_bits( 0xE1, 4, 4, &extracted_bits ) );
  assert( 0x0E == extracted_bits );

  assert( 0 == byte_get_bits( 0xE1, 0, 8, &extracted_bits ) );
  assert( 0xE1 == extracted_bits );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Tenter d'extraire plus de bit que 8 est bien considéré comme une erreur :\e[m\n" );
  assert( 0 != byte_get_bits( 0xA8, 5, 4, &extracted_bits ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Passer NULL en dernier paramètre est bien détecté comme une erreur :\e[m\n" );
  assert( 0 != byte_get_bits( 0xA8, 0, 1, NULL ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}



int main ( ) {
  test_byte_get_lsb();
  test_byte_get_bits();

  exit( EXIT_SUCCESS );
}
