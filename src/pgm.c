#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include <steg1ano/pgm.h>


image_t pgm_read( char *filename ) {
  // TODO
  FILE *file = NULL;
  image_t img = NULL;

  /* Vérification des paramètres */
  if (filename == NULL) {
    return NULL;
  }

  /* Ouverture du fichier */
  file = fopen(filename, "rb");

  /* Vérification de l'ouverture */
  if (file == NULL) {
    return NULL;
  }

  /* Lecture de l'en-tête */
  char header[3];
  if (fscanf(file, "%s", header) != 1) {
    return NULL;
  }

  /* Vérification de l'en-tête */
  if (strcmp(header, "P5") != 0) {
    return NULL;
  }

  /* Lecture de la largeur */
  int width;
  if (fscanf(file, "%d", &width) != 1) {
    return NULL;
  }

  /* Lecture de la hauteur */
  int height;
  if (fscanf(file, "%d", &height) != 1) {
    return NULL;
  }

  /* Lecture de la valeur maximale */
  int max;
  if (fscanf(file, "%d", &max) != 1) {
    return NULL;
  }

  /* Vérification de la valeur maximale */
  if (max != 255) {
    return NULL;
  }

  /* Allocation de la mémoire */
  img = image_new(height, width, 1);

  /* Vérification de l'allocation */
  if (img == NULL) {
    return NULL;
  }

  /* Sauter les commentaires */
  char s[1024];
  do { fgets(s,1023,file); } while (s[0]=='#');

  /* Lecture (binaire) des pixels */
  for (int i = 0; i < height*width; i++) {
    if (fread(&(img->pix[0][i]), sizeof(pixel_t), 1, file) != 1) {
      return NULL;
    }
  }

  /* Fermeture du fichier */
  if (fclose(file) != 0) {
    return NULL;
  }

  return img;
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