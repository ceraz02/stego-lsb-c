#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include <steg1ano/options.h>

char *option_string( char letter, int argc, char *argv[] ) {
  // TODO
  int i;
  for (i = 0; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == letter) {
      if (argv[i][2] == '=') {
        return argv[i] + 3;
      }
      else {
        return argv[i] + 2;
      }
    }
  }
  return NULL;
}



int option_int( char sopt, int argc, char *argv[], int *p_val ) {
  char     *opt, *endptr;
  long int  val = -1;

  if( NULL == p_val) {
    return -1;
  }

  opt = option_string( sopt, argc, argv );

  if ( NULL == opt || '\0' == *opt ) {
    return -1;
  }

  errno = 0;
  val = strtol( opt, &endptr, 0 );

  if ( ( errno == ERANGE && ( val == LONG_MAX || val == LONG_MIN ) ) || ( errno != 0 && val == 0 ) ||
       endptr == opt || *endptr ) {
    fprintf( stderr, "ERROR option_int : En lisant un entier: Caractères inattendus: '%s'.\n", opt );
    return -1;
  }

  *p_val = val;

  return 0;
}

int      option_flag( char sopt, int argc, char *argv[] ) {
  if ( NULL == option_string( sopt, argc, argv ) ) {
    return 0;

  }
  return 1;
}
