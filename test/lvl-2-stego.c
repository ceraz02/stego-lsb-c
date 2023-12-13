#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include <steg1ano/stego.h>
#include <steg1ano/pgm.h>
#include <steg1ano/byte.h>

#define MESSAGE "Il fait beau et chaud"
// Rq : le premier octet, caractère 'I', a pour valeur ASCII 0x49

void test_stego_encode_block( image_t img, int b ) {
  int       row = 0;
  int       col = 0;

  fprintf( stderr, "\e[36m** Test de la fonction %s avec b=%d\e[m\n", __func__+5, b );

  fprintf( stderr, " \e[32m<= Test : On peut cacher un bloc d'octets dans une image: \e[m\n" );
  printf("Message encodé : %s\n", MESSAGE);
  assert( 0 == stego_encode_block( (byte_t *)MESSAGE, strlen( MESSAGE ), img, &row, &col, b ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
  
  fprintf( stderr, " \e[32m<= Test : Le premier pixel qui a été modifié a les bons LSB : \n" );
  // les b bits de poids faible du premier pixel sont-ils corrects ?
  // Ils doivent égaux aux b bits de poids faible du caractère 'I'
  byte_t lsb_first_pixel, lsb_first_letter;
  byte_get_lsb( img->pix[0][0], b, &lsb_first_pixel );
  byte_get_lsb( MESSAGE[0], b, &lsb_first_letter );
  assert( lsb_first_pixel == lsb_first_letter );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}

void test_stego_decode_block( image_t img, int b ) {
  char      dec[ 256 ];
  int       row = 0;
  int       col = 0;

  fprintf( stderr, "\e[36m** Test de la fonction %s avec b=%d\e[m\n", __func__+5, b );

  memset( dec, 0, sizeof( dec ) );

  fprintf( stderr, " \e[32m<= Test : On peut extraire un bloc d'octets caché dans une image: \e[m\n" );
  assert( 0 == stego_decode_block( (byte_t *)dec, strlen( MESSAGE ), img, &row, &col, b ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Le bloc a été correctement relu : \e[m\n" );
  printf("Message extrait : %s\n", dec);
  assert( 0 == strcmp( dec, MESSAGE ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}

#undef MESSAGE


void test_stego_decode_analyse_canary() {
  image_t img ;
  int ret ;
  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );
  
  // Le canary vaut (cf. stego.c)
  //    0xefbeadde
  // c'est à dire les octets dans l'ordre
  //    0xde 0xad 0xbe 0xef 

  fprintf( stderr, " \e[32m<= Test : On peut retrouver le canary caché avec 8 bits par pixels: \e[m\n" );
  img = image_new( 99, 50, 1) ; // une image en niveau de gris toute noire
  assert( NULL != img);
  ret = 0;
  // cachons "à la main" le canary avec 8 bits par pixels
  ret += image_set_pixel(img, 0, 0, 0xde);
  ret += image_set_pixel(img, 0, 1, 0xad);
  ret += image_set_pixel(img, 0, 2, 0xbe);
  ret += image_set_pixel(img, 0, 3, 0xef);
  assert( 0 == ret );
  assert( 8 == stego_decode_analyse_canary( img ) );
  image_delete(img);
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : On peut retrouver le canary caché avec 4 bits par pixels: \e[m\n" );
  img = image_new( 99, 50, 1) ; // une image en niveau de gris toute noire
  ret = 0;
  // cachons "à la main" le canary 0xefbeadde avec 4 bits par pixels
  ret += image_set_pixel(img, 0, 0, 0x0e);
  ret += image_set_pixel(img, 0, 1, 0x0d);
  ret += image_set_pixel(img, 0, 2, 0x0d);
  ret += image_set_pixel(img, 0, 3, 0x0a);
  ret += image_set_pixel(img, 0, 4, 0x0e);
  ret += image_set_pixel(img, 0, 5, 0x0b);
  ret += image_set_pixel(img, 0, 6, 0x0f);
  ret += image_set_pixel(img, 0, 7, 0x0e);
  assert( 0 == ret );
  assert( 4 == stego_decode_analyse_canary( img ) );
  image_delete(img);
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : On peut retrouver le canary caché avec 2 bits par pixels: \e[m\n" );
  img = image_new( 99, 50, 1) ; // une image en niveau de gris toute noire
  ret = 0;
  // cachons "à la main" le canary 0xefbeadde avec 2 bits par pixels
  ret += image_set_pixel(img, 0, 0,  0x02);
  ret += image_set_pixel(img, 0, 1,  0x03);
  ret += image_set_pixel(img, 0, 2,  0x01);
  ret += image_set_pixel(img, 0, 3,  0x03);
  ret += image_set_pixel(img, 0, 4,  0x01);
  ret += image_set_pixel(img, 0, 5,  0x03);
  ret += image_set_pixel(img, 0, 6,  0x02);
  ret += image_set_pixel(img, 0, 7,  0x02);
  ret += image_set_pixel(img, 0, 8,  0x02);
  ret += image_set_pixel(img, 0, 9,  0x03);
  ret += image_set_pixel(img, 0, 10, 0x03);
  ret += image_set_pixel(img, 0, 11, 0x02);
  ret += image_set_pixel(img, 0, 12, 0x03);
  ret += image_set_pixel(img, 0, 13, 0x03);
  ret += image_set_pixel(img, 0, 14, 0x02);
  ret += image_set_pixel(img, 0, 15, 0x03);
  assert( 0 == ret );
  assert( 2 == stego_decode_analyse_canary( img ) );
  image_delete(img);
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
  

  fprintf( stderr, " \e[32m<= Test : On peut retrouver un canary caché avec 1 bits par pixels: \e[m\n" );
  img = image_new( 99, 50, 1) ; // une image en niveau de gris toute noire
  ret = 0;
  // cachons "à la main" le canary 0xefbeadde avec 1 bits par pixels
  ret += image_set_pixel(img, 0, 0,  0x00);
  ret += image_set_pixel(img, 0, 1,  0x01);
  ret += image_set_pixel(img, 0, 2,  0x01);
  ret += image_set_pixel(img, 0, 3,  0x01);
  ret += image_set_pixel(img, 0, 4,  0x01);
  ret += image_set_pixel(img, 0, 5,  0x00);
  ret += image_set_pixel(img, 0, 6,  0x01);
  ret += image_set_pixel(img, 0, 7,  0x01);
  ret += image_set_pixel(img, 0, 8,  0x01);
  ret += image_set_pixel(img, 0, 9,  0x00);
  ret += image_set_pixel(img, 0, 10, 0x01);
  ret += image_set_pixel(img, 0, 11, 0x01);
  ret += image_set_pixel(img, 0, 12, 0x00);
  ret += image_set_pixel(img, 0, 13, 0x01);
  ret += image_set_pixel(img, 0, 14, 0x00);
  ret += image_set_pixel(img, 0, 15, 0x01);
  ret += image_set_pixel(img, 0, 16, 0x00);
  ret += image_set_pixel(img, 0, 17, 0x01);
  ret += image_set_pixel(img, 0, 18, 0x01);
  ret += image_set_pixel(img, 0, 19, 0x01);
  ret += image_set_pixel(img, 0, 20, 0x01);
  ret += image_set_pixel(img, 0, 21, 0x01);
  ret += image_set_pixel(img, 0, 22, 0x00);
  ret += image_set_pixel(img, 0, 23, 0x01);
  ret += image_set_pixel(img, 0, 24, 0x01);
  ret += image_set_pixel(img, 0, 25, 0x01);
  ret += image_set_pixel(img, 0, 26, 0x01);
  ret += image_set_pixel(img, 0, 27, 0x01);
  ret += image_set_pixel(img, 0, 28, 0x00);
  ret += image_set_pixel(img, 0, 29, 0x01);
  ret += image_set_pixel(img, 0, 30, 0x01);
  ret += image_set_pixel(img, 0, 31, 0x01);
  assert( 0 == ret );
  assert( 1 == stego_decode_analyse_canary( img ) );
  image_delete(img);
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  
  fprintf( stderr, " \e[32m<= Test : Si il n'est pas caché, la fonction retourne bien un nombre < 0 : \e[m\n" );
  img = image_new( 99, 50, 1) ; // une image en niveau de gris toute noire
  ret = 0;
  // on met des pixels "aléatoires" sur la première ligne de l'image
  srand(time(NULL));
  int col;
  for(col = 0 ; col < 50 ; col ++) {
    ret += image_set_pixel(img, 0, col, rand()%256);
  }
  assert( 0 == ret );
  assert( stego_decode_analyse_canary( img ) < 0);
  image_delete(img);
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}

int main ( ) {

  image_t img ;
  
  img = pgm_read( "image/basic.pgm" );
  test_stego_encode_block(  img, 8 );
  test_stego_decode_block(  img, 8 );
  image_delete( img );
  
  img = pgm_read( "image/basic.pgm" );
  test_stego_encode_block(  img, 4 );
  test_stego_decode_block(  img, 4 );
  image_delete( img );
  
  img = pgm_read( "image/basic.pgm" );
  test_stego_encode_block(  img, 2 );
  test_stego_decode_block(  img, 2 );
  image_delete( img );
  
  img = pgm_read( "image/basic.pgm" );
  test_stego_encode_block(  img, 1 );
  test_stego_decode_block(  img, 1 );
  image_delete( img );
  
  test_stego_decode_analyse_canary();

  exit( EXIT_SUCCESS );
}
