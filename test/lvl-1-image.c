#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <steg1ano/image.h>

void test_image_get_height( void ) {
  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : Une image NULL a une hauteur de zéro pixel: \e[m \n" );
  assert( 0 == image_get_height( NULL ) );

  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}

void test_image_get_width( void ) {
  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : Une image NULL a une largeur de zéro pixel: \e[m \n" );
  assert( 0 == image_get_width( NULL ) );

  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}

void test_image_new( void ) {

  image_t img = NULL;

  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : Une image a 1 ou 3 canaux colorimétriques: \e[m \n" );
  img = image_new( 42, 51, 1 );
  assert( img );
  image_delete( img );
  img = image_new( 42, 51, 3 );
  assert( img );
  image_delete( img );
  // on ne peut pas créer une image avec 2 canaux chromatiques
  img = image_new( 42, 51, 2 );
  assert( NULL == img );
  image_delete( img );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Une nouvelle image a les bonnes dimensions:\n" );
  img = image_new( 112, 54, 1 );
  assert( 112 == image_get_height( img ) );
  assert( 54 == image_get_width( img ) );
  image_delete( img );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
  
  fprintf( stderr, " \e[32m<= Test : Les pixels de l'image sont bien alloués de façon contigue:\n" );
  // on vérifie ici que les adresse de début des lignes
  // sont séparés
  img = image_new( 112, 53, 1 );
  assert( img->pix[1] - img->pix[0]     == 53 * sizeof(pixel_t));
  assert( img->pix[111] - img->pix[0]   == 53 * 111 * sizeof(pixel_t) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  image_delete( img );

  fprintf( stderr, " \e[32m<= Test : Créer une image avec un nombre de ligne < 0 est bien considéré comme une erreur: \e[m\n" );
  img = image_new( -5, 51, 1 );
  assert( NULL == img );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}


void test_image_delete( void ) {
  int ret ;

  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : Appeler image_delete si le pointeur img vaut NULL ne fait rien \e[m \n" );
  ret = image_delete( NULL );
  assert( 0 != ret );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}


void test_image_get_pixel( ) {

  image_t img =  image_new( 42, 51, 1 );

  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : Accéder à un pixel d'une image NULL retourne -1: \e[m \n" );
  assert( -1 == image_get_pixel( NULL, 0, 0, NULL ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Accéder à un pixel hors dimensions d'une image retourne -1: \e[m \n" );
  assert( -1 == image_get_pixel( img, image_get_height( img ), 0, NULL ) );
  assert( -1 == image_get_pixel( img, 0, image_get_width( img ), NULL ) );
  assert( -1 == image_get_pixel( img, -1, 0, NULL ) );
  assert( -1 == image_get_pixel( img, 0, -1, NULL ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  image_delete( img );
}

void test_image_set_pixel( ) {

  image_t img =  image_new( 3, 5, 1 );
  pixel_t pix;
  int ret ;

  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : Accéder en écriture à un pixel d'une image NULL retourne -1:\e[m \n" );
  assert( -1 == image_set_pixel( NULL, 2, 3, 155 ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Accéder en écriture à un pixel hors dimensions d'une image retourne un nombre non nul : \e[m \n" );
  assert( 0 != image_set_pixel( img, image_get_height( img ), 0, 155 ) );
  assert( 0 != image_set_pixel( img, 0, image_get_width( img ),  155 ) );
  assert( 0 != image_set_pixel( img, -1, 0,  155 ) );
  assert( 0 != image_set_pixel( img, 0, -1,  155 ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Accéder en lecture à un pixel hors dimensions d'une image retourne un nombre non nul : \e[m \n" );
  assert( 0 != image_get_pixel( img, image_get_height( img ), 0, &pix ) );
  assert( 0 != image_get_pixel( img, 0, image_get_width( img ),  &pix ) );
  assert( 0 != image_get_pixel( img, -1, 0,  &pix ) );
  assert( 0 != image_get_pixel( img, 0, -1,  &pix ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Test de lecture/écriture d'un pixel:\e[m \n" );
  ret = image_set_pixel( img, 2, 3, 155);
  assert( 0 == ret );
  ret = image_get_pixel(img, 2, 3, &pix);
  assert( 0 == ret );
  assert( 155 == pix );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  image_delete( img );
}

int main ( ) {

  test_image_new();

  test_image_get_height();
  test_image_get_width();

  test_image_get_pixel();
  test_image_set_pixel();

  exit( EXIT_SUCCESS );
}
