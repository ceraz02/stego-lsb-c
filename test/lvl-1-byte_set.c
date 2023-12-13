#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <steg1ano/byte.h>

void test_byte_set_lsb( void ) {
  byte_t v ;

  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : On peut écrire les LSB d'un octet:\e[m\n" );
  v = 0xFF ;
  assert( 0 == byte_set_lsb( &v, 8, 0xAA ) );
  assert( 0xAA == v );
  v = 0xFF ;
  assert( 0 == byte_set_lsb( &v, 4, 0xAA ) );
  assert( 0xFA == v );
  v = 0xFF ;
  assert( 0 == byte_set_lsb( &v, 2, 0xAA ) );
  assert( 0xFE == v );
  v = 0xAA ;
  assert( 0 == byte_set_lsb( &v, 1, 0xF1 ) );
  assert( 0xAB == v );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Affecter plus de 8 bits est bien détecté comme une erreur :\e[m\n" );
  assert( 0 != byte_set_lsb(  &v, 9, 0xaa ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}



void test_byte_set_bits( void ) {
  byte_t byte = 0;

  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : On peut affecter une serie de bits dans octet :\e[m\n" );
  byte = 0x26;
  assert( 0 == byte_set_bits( &byte, 0, 1, 0x01 ) );
  printf("byte %hhx\n", byte);
  assert( 0x27 == byte );

  byte = 0x26;
  assert( 0 == byte_set_bits( &byte, 4, 1, 0x01 ) );
  assert( 0x36 == byte );

  byte = 0x26;
  assert( 0 == byte_set_bits( &byte, 5, 1, 0x01 ) );
  assert( 0x26 == byte );
  
  byte = 0x26;
  assert( 0 == byte_set_bits( &byte, 6, 1, 0x01 ) );
  assert( 0x66 == byte );
  
  byte = 0x26;
  assert( 0 == byte_set_bits( &byte, 7, 1, 0x01 ) );
  assert( 0xA6 == byte );

  byte = 0xF8;
  assert( 0 == byte_set_bits( &byte, 0, 2, 0x03 ) );
  assert( 0xFB == byte );

  byte = 0x47;
  assert( 0 == byte_set_bits( &byte, 4, 2, 0x03 ) );
  assert( 0x77 == byte );

  byte = 0x27;
  assert( 0 == byte_set_bits( &byte, 6, 2, 0x03 ) );
  assert( 0xE7 == byte );

  byte = 0xF4;
  assert( 0 == byte_set_bits( &byte, 0, 4, 0x0B ) );
  assert( 0XFB == byte );

  byte = 0x27;
  assert( 0 == byte_set_bits( &byte, 4, 4, 0x0A ) );
  assert( 0XA7 == byte );

  byte = 0x27;
  assert( 0 == byte_set_bits( &byte, 0, 8, 0xAB ) );
  assert( 0XAB == byte );

  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
  
  fprintf( stderr, " \e[32m<= Test : Tenter d'écrire plus de bit que 8 est bien considéré comme une erreur :\e[m\n" );
  assert( 0 != byte_set_bits( &byte, 2, 7, 0xAB ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Passer NULL en premier paramètre est bien détecté comme une erreur :\e[m\n" );
  assert( 0 != byte_set_bits( NULL, 0, 2, 0xAB ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}



int main ( ) {
  test_byte_set_lsb();
  test_byte_set_bits();
  
  exit( EXIT_SUCCESS );
}
