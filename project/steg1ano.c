#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <steg1ano/stego.h>
#include <steg1ano/options.h>
#include <steg1ano/pgm.h>

void error_usage( char *progname ) {
  char *p = progname + strlen( progname );

  while ( p > progname && '/' != *p ) p--;

  progname = p+1;

  fprintf( stderr, "\n" );
  fprintf( stderr, " %s - Stéganographie LSB en image.\n", progname );
  fprintf( stderr, "\n" );

  fprintf( stderr, " SYNOPSIS\n" );
  fprintf( stderr, "\n" );

  fprintf( stderr, "  %s encode image.pgm foo.dat stego.pgm [OPTIONS]\n", progname );
  fprintf( stderr, "  %s decode image.pgm\n", progname );

  fprintf( stderr, "\n" );
  fprintf( stderr, " OPTIONS\n" );
  fprintf( stderr, "\n" );
  fprintf( stderr, "  -b=1..8   Nombre de LSB des pixels à utiliser, par défault: 2.\n" );
  fprintf( stderr, "            Hors bonus, on se limitera au cas 1, 2, 4 ou 8\n" );
  fprintf( stderr, "  -h        Affiche ce message.\n" );
  fprintf( stderr, "\n" );
}

int command_encode( char *cover, char *stego, char *payload, int b ) {
  int      ret = -1;
  image_t  img;

  img = pgm_read( cover );

  if ( NULL == img ) {
    return -1;
  }

  ret = stego_encode( img, payload, b );

  if ( -1 != ret ) {
    ret = pgm_write( stego, img );
  }

  image_delete( img );

  return ret;
}

int command_decode( char *cover ) {
  int      ret = -1;
  image_t  img = NULL;

  img = pgm_read( cover );

  if ( NULL == img ) {
    return -1;
  }

  ret = stego_decode( img );

  image_delete( img );

  return ret;
}

int main ( int argc, char *argv[] ) {
  
  if ( argc < 3 || option_flag( 'h', argc, argv ) ) {
    error_usage( argv[ 0 ] );
    exit( EXIT_SUCCESS );
  }

  if ( !strcmp( argv[ 1 ], "encode" ) && argc > 4 ) {
    int b = 2;

    option_int( 'b', argc, argv, &b );

    exit( command_encode( argv[ 2 ], argv[ 4 ], argv[ 3 ], b ) );
  }

  if ( !strcmp( argv[ 1 ], "decode" ) && argc > 2 ) {
    exit( command_decode( argv[ 2 ] ) );
  }

  error_usage( argv[ 0 ] );

  exit( EXIT_FAILURE );
}

