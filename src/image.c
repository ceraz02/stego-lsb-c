#include <stdlib.h>
#include <stdio.h>

#include <steg1ano/image.h>

image_t  image_new( int height, int width, int nbchan ) {
  // TODO

  /* Vérification des paramètres */
  if ((height < 0) || (width < 0))
  {
    // printf("Erreur : height ou width < 0\n");
    return NULL;
  }
  if ((nbchan != 1) && (nbchan != 3)) 
  {
    // printf("Erreur : nbchan != 1 ou 3\n");
    return NULL;
  }

  /* Allocation de la mémoire */
  image_t img = calloc(1, sizeof(*img));
  if (img == NULL) 
  {
    // printf("Erreur d'allocation de mémoire\n");
    return NULL;
  }

  /* Initialisation des champs */
  img->height = height;
  img->width = width;
  img->nbchan = nbchan;

  /* Allocation de la mémoire pour les pixels */
  img->pix = calloc(height, sizeof(pixel_t *));
  if (img->pix == NULL) return NULL;

  /* Allocation de la mémoire pour les lignes */
  img->pix[0] = calloc(height * width, sizeof(pixel_t));
  if (img->pix[0] == NULL) return NULL;

  /* Initialisation des lignes */
  for (int i = 1; i < height; i++) {
    img->pix[i] = img->pix[0] + i * width;
  }

  return img;
}

int      image_delete( image_t img ) {
  // TODO
  if (img == NULL) return 1;

  free(img->pix[0]);
  free(img->pix);
  free(img);

  return 0;
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

  /* Vérification des paramètres */
  if (img == NULL) return -1;
  if ((r < 0) || (r >= img->height) || (c < 0) || (c >= img->width)) return -1;

  /* Récupération du pixel */
  *p_pix = img->pix[r][c];

  // printf("r = %d, c = %d, pix = %d\n", r, c, *p_pix);

  return 0;
}

int image_set_pixel( image_t img, int r, int c, pixel_t pix ) {
  // TODO

  /* Vérification des paramètres */
  if (img == NULL) return -1;
  if ((r < 0) || (r >= img->height) || (c < 0) || (c >= img->width)) return -1;

  /* Modification du pixel */
  img->pix[r][c] = pix;

  return 0;
}
