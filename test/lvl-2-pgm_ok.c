#define _XOPEN_SOURCE 200809L
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <steg1ano/pgm.h>

// Test de la lecture / écriture de fichiers image PGM
// respectant le format PGM et n'incluant pas de commentaire.
// Des tests de fichiers PGM invalides ou avec commentaire
// sont dans lvl-3-pgm_read_detects_invalid_file.c 
// et lvl-3-pgm_read_with_comments.c

image_t test_pgm_read( char * filename, int awaitedHeight, int awaitedWidth ) {
  image_t img;

  fprintf( stderr, "\e[36m** Test de la fonction %s sur le fichier image %s\e[m\n", __func__+5, filename );

  fprintf( stderr, " \e[32m<= Test : On peut lire l'image PGM binaire %s : \e[m\n", filename );
  img = pgm_read( filename );
  assert( NULL != img );
  assert( awaitedHeight == image_get_height( img ) );
  assert( awaitedWidth == image_get_width( img ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  return img;
}

// vérifie que le pixel i, j vaut bien la valeur attendue awaitedPixelValue
void assert_image_get_pixel( image_t img, int i, int j, int awaitedPixelValue ) {
  pixel_t pix;
  int ret;

  fprintf( stderr, " \e[32m<= Test : Le pixel %d %d a la valeur attendue : \e[m\n", i, j);
  ret = image_get_pixel(img, i, j, &pix);

  assert( 0 ==  ret);
  assert( awaitedPixelValue ==  pix);
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}

void test_image_print( image_t img ) {
  int ret;
  ret = image_printf(img);
  assert(ret > 0);
}

void test_pgm_write( image_t img ) {
  image_t img_saved = NULL;
  pixel_t pix1, pix2;
  int ret;

  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : On peut écrire une image PGM binaire: \e[m\n" );
  assert( -1 != pgm_write( "image/test.pgm", img ) );

  // on relit l'image qu'on vient de sauver
  img_saved = pgm_read( "image/test.pgm" );

  assert( img );
  assert( image_get_height(img) == image_get_height(img_saved) );
  assert( image_get_width(img) == image_get_width(img_saved) );
  ret = image_get_pixel(img, image_get_height(img)/2, image_get_width(img)/3, &pix1);
  assert( 0 ==  ret);
  ret = image_get_pixel(img_saved, image_get_height(img_saved)/2, image_get_width(img_saved)/3, &pix2);
  assert( pix1 ==  pix2);
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  image_delete( img_saved );

  unlink( "image/test.pgm" ); // destruction du fichier image/test.pgm
}

int main ( ) {
  image_t img = NULL;

  img =  test_pgm_read( "image/basic.pgm", 10, 20 );
  // on verifie que quelques-uns des pixels a la valeur attendue
  assert_image_get_pixel( img, 0, 0, 0 );
  assert_image_get_pixel( img, 0, 19, 122 );
  assert_image_get_pixel( img, 9, 0, 65 );
  assert_image_get_pixel( img, 9, 19, 255 );
  test_image_print( img );
  // on vérifie qu'on peut réécrire l'image
  test_pgm_write( img );
  image_delete( img );

  // une image avec width==height
  img =  test_pgm_read( "image/lena.pgm", 512, 512);
  // on verifie que quelques-uns des pixels a la valeur attendue
  assert_image_get_pixel( img, 0, 0, 162);
  assert_image_get_pixel( img, 42, 51, 162);
  assert_image_get_pixel( img, 55, 112, 120);
  // on vérifie qu'on peut réécrire l'image
  test_pgm_write( img );
  image_delete( img );

  // une plus grande image rectangle
  img =  test_pgm_read( "image/dauphin.pgm", 422, 600 );
  // on verifie que quelques-uns des pixels a la valeur attendue
  assert_image_get_pixel( img, 12, 598, 137 );
  assert_image_get_pixel( img, 421, 599, 34 );
  // on vérifie qu'on peut réécrire l'image
  test_pgm_write( img );
  image_delete( img );

  exit( EXIT_SUCCESS );
}
