#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <steg1ano/options.h>

void test_option_string( void ) {

  char *argv[] ={ "fake_command", "arg", "-k", "-v=3", "-l=" };
  int   argc   = sizeof( argv ) / sizeof( argv[ 0 ] );

  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : Une option absente retourne NULL :\e[m\n" );
  assert( NULL == option_string( 'z', argc, argv ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Une option présente sans valeur retourne non-NULL: \e[m\n" );
  assert( option_string( 'k', argc, argv ) );
  assert( option_string( 'l', argc, argv ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Une option présente avec valeur retourne la chaîne passée en paramètre : \e[m\n" );
  assert( 0 == strcmp( "3", option_string( 'v', argc, argv ) ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}

void   test_option_int( ) {
  char *argv[] ={ "fake_command", "arg", "-b=125", "-c", "-d=zzzzz", "-e=" };
  int   argc   = sizeof( argv ) / sizeof( argv[ 0 ] );

  int val;

  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : Une option absente retourne -1: \e[m\n" );
  assert( -1 == option_int( 'z', argc, argv, &val ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Une option avec valeur non entière retourne -1: \e[m\n" );
  assert( -1 == option_int( 'c', argc, argv, &val ) );
  assert( -1 == option_int( 'd', argc, argv, &val ) );
  assert( -1 == option_int( 'e', argc, argv, &val ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Une option avec valeur entiere est bien trouvée: \e[m\n" );
  assert( 0 == option_int( 'b', argc, argv, &val ) );
  assert( 125 == val );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}



void   test_option_flag( ) {
  char *argv[] ={ "fake_command", "arg", "-b=125", "-c", "-d=zzzzz", "-e=" };
  int   argc   = sizeof( argv ) / sizeof( argv[ 0 ] );

  fprintf( stderr, "\e[36m** Test de la fonction %s\e[m\n", __func__+5 );

  fprintf( stderr, " \e[32m<= Test : Une option absente retourne -1: \e[m\n" );
  assert( ! option_flag( 'z', argc, argv) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );

  fprintf( stderr, " \e[32m<= Test : Une option présente est bien détectée: \e[m\n" );
  assert( option_flag( 'b', argc, argv ) );
  assert( option_flag( 'c', argc, argv ) );
  assert( option_flag( 'd', argc, argv ) );
  assert( option_flag( 'e', argc, argv ) );
  fprintf( stderr, " \e[32m=> Test passé avec succès\e[m\n" );
}



int main ( ) {

  test_option_string();
  test_option_int();
  test_option_flag();

  exit( EXIT_SUCCESS );
}
