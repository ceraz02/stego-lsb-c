#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include <steg1ano/pgm.h>


image_t pgm_read( char *filename ) {
  // TODO
  return NULL;
}



int     pgm_write( char *filename, image_t img ) {
  int height, width;
  int     ret = 0;
    
  if( NULL == img ) {
    return -1 ;
  }
  if( 1 != img->nbchan ) {
    return -2 ;
  }
  
  FILE *fp = fopen( filename, "w" );
  if ( NULL == fp ) {
    fprintf( stderr, "ERROR pgm_write : Impossible d'ouvrir %s: %s.\n", filename, strerror( errno ) );
    return -3;
  }
  ret += fprintf( fp, "P5\n" );
  
  ret   += fprintf( fp, "%d %d\n", img->width, img->height  );
  ret   += fprintf( fp, "%d\n", 255 );
  
  height = image_get_height( img );
  width  = image_get_width( img );
  
  ret = fwrite( img->pix[ 0 ], sizeof( img->pix[ 0 ][ 0 ] ), image_get_width( img ) * image_get_height( img ), fp );
  if ( ret < image_get_width( img ) * image_get_height( img ) ) {
    fprintf( stderr, "ERROR pgm_write : Impossible d'écrire les valeurs des pixels: %s (%d/%d pixels écrits).\n", strerror( errno ), ret, image_get_width( img ) * image_get_height( img ) );
    fclose( fp );
    return -4;
  }
  
  fclose( fp );
  
  return ret;
}