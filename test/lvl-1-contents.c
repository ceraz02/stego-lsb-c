#define _POSIX_C_SOURCE 200809L
#include <unistd.h> /* unlink */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <steg1ano/contents.h>

#define FILENAME "test-lvl1-contents.data"
#define CONTENTS "Le monde entier est un cactus"

void test_content_write() {

  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : On peut écrire le contenu d'un fichier:\e[m\n" );
  assert( 0 == contents_write( FILENAME, (void*)CONTENTS, 1 + strlen( CONTENTS ) ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès. Le fichier %s a bien été créé.\e[m\n", FILENAME );
}

void test_content_read() {
  byte_t *contents;
  size_t      size;

  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : On peut relire le contenu d'un fichie:\e[m\n" );
  contents = contents_read( FILENAME, &size );
  assert( contents );
  assert( strlen( CONTENTS ) == size-1 );
  assert( 0 == strncmp( CONTENTS, (char*)contents, strlen( CONTENTS ) ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Passer un poiteur NULL pour le paramètre p_size est bien détecté comme une erreur : \e[m \n" );
  contents = contents_read( FILENAME, NULL );
  assert( NULL == contents );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );


  free( contents );
}

int main ( ) {

  test_content_write( );
  test_content_read( );
  unlink( FILENAME );

  exit( EXIT_SUCCESS );
}

#undef FILENAME
#undef CONTENTS
