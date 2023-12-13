#include <stdlib.h>
#include <stdio.h>

#include <steg1ano/image.h>

image_t  image_new( int height, int width, int nbchan ) {
  // TODO
  return NULL;
}

int      image_delete( image_t img ) {
  // TODO
  return -1;
}

int        image_printf( image_t img ) {
  int  ret = 0;
  int  row;

  if ( NULL == img ) {
    fprintf( stderr, "ERROR image_new : *PAS* d'image ici!" );
    return -1;
  }

  ret += printf( "Image: %dx%dpx\n", img->width, img->height );

  for ( row = 0 ; row < img->height ; row++ ) {
    int col;
    for ( col = 0 ; col < img->width ; col++ ) {
      int chan;
      pixel_t v;
      for ( chan = 0 ; chan < img->nbchan-1 ; chan++ ) {
        image_get_pixel( img, row, col+chan, &v );
        ret += printf( "%03hhu.", v );
      }
      image_get_pixel( img, row, col+chan, &v );
      ret += printf( "%03hhu ", v );
    }
    ret += printf("\n");
  }

  return ret;
}

int      image_get_height( image_t img ) {
  if ( NULL == img ) {
    return 0;
  }

  return img->height;
}

int      image_get_width( image_t img ) {
  if ( NULL == img ) {
    return 0;
  }

  return img->width;
}

int      image_get_nbchan( image_t img ) {
  if ( NULL == img ) {
    return 0;
  }

  return img->nbchan;
}

int image_get_pixel( image_t img, int r, int c, pixel_t * p_pix ) {
  // TODO
  return -1;
}

int image_set_pixel( image_t img, int r, int c, pixel_t pix ) {
  // TODO
  return -1;
}
