#define _XOPEN_SOURCE 200809L
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <steg1ano/pgm.h>


void test_pgm_read_detects_invalid_pgm_files() {
  image_t img;

  fprintf( stderr, "\e[36m** Test du fait que la fonction test_pgm_read détecte si un fichier PGM est invalide \e[m\n");

  fprintf( stderr, " \e[32m<= Test : Lire un fichier PGM qui n'existe pas est bien considéré comme une erreur : \e[m\n" );
  img = pgm_read( "image/pgm_test_lvl-3/file_that_does_not_exist.pgm" );
  assert( NULL == img);
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Lire un fichier PGM où il manque P5 est bien considéré comme une erreur : \e[m\n" );
  img = pgm_read( "image/pgm_test_lvl-3/erreur_invalid_pgm_header_P5.pgm" );
  assert( NULL == img);
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Lire un fichier PGM où il manque le nombre de lignes est bien considéré comme une erreur : \e[m\n" );
  img = pgm_read( "image/pgm_test_lvl-3/erreur_missing_number_line.pgm" );
  assert( NULL == img);
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Lire un fichier PGM sans 3 entiers textuels est bien considéré comme une erreur : \e[m\n" );
  img = pgm_read( "image/pgm_test_lvl-3/erreur_not_txt_integers.pgm" );
  assert( NULL == img);
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Lire un fichier PGM dans lequel il manque des pixels est bien considéré comme une erreur : \e[m\n" );
  img = pgm_read( "image/pgm_test_lvl-3/erreur_not_enough_pixels.pgm" );
  assert( NULL == img);
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}

int main ( ) {
  test_pgm_read_detects_invalid_pgm_files();

  exit( EXIT_SUCCESS );
}
