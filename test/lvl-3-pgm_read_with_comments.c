#define _XOPEN_SOURCE 200809L
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <steg1ano/pgm.h>


image_t test_pgm_read_with_comments( char * filename, int awaitedHeight, int awaitedWidth ) {
  image_t img;

  fprintf( stderr, "\e[36m** Test de la fonction %s sur le fichier image %s\e[m\n", __func__+5, filename );

  fprintf( stderr, " \e[32m<= Test : On peut lire l'image PGM binaire contenant des commentaires %s : \e[m\n", filename );
  img = pgm_read( filename );
  assert( NULL != img );
  assert( awaitedHeight == image_get_height( img ) );
  assert( awaitedWidth == image_get_width( img ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  return img;
}

// vérifie que le pixel i, j vaut bien la valeur attendue awaitedPixelValue
void assert_pixel_value( image_t img, int i, int j, int awaitedPixelValue ) {
  pixel_t pix;
  int ret;

  fprintf( stderr, " \e[32m<= Test : Le pixel %d %d a la valeur attendue : \e[m\n", i, j);
  ret = image_get_pixel(img, i, j, &pix);

  assert( 0 ==  ret);
  assert( awaitedPixelValue ==  pix);
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}


int main ( ) {
  image_t img = NULL;

  img =  test_pgm_read_with_comments( "image/pgm_test_lvl-3/lena_with_comments.pgm", 512, 512);
  // on verifie que quelques-uns des pixels a la valeur attendue
  assert_pixel_value( img, 0, 0, 162);
  assert_pixel_value( img, 42, 51, 162);
  assert_pixel_value( img, 55, 112, 120);

  exit( EXIT_SUCCESS );
}
