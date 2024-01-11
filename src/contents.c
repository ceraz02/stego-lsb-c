#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <steg1ano/contents.h>

byte_t *contents_read( char *filename, size_t *p_size ) {
  // TODO
  FILE *file = NULL;
  byte_t *contents = NULL;
  size_t size = 0;

  /* Vérification des paramètres */
  if (p_size == NULL) {
    return NULL;
  }

  /* Ouverture du fichier */
  file = fopen(filename, "rb");

  /* Vérification de l'ouverture */
  if (file == NULL) {
    return NULL;
  }

  /* Récupération de la taille du fichier */
  fseek(file, 0, SEEK_END);
  size = ftell(file);
  fseek(file, 0, SEEK_SET);

  /* Allocation de la mémoire */
  contents = malloc(sizeof(byte_t) * size);

  /* Vérification de l'allocation */
  if (contents == NULL) {
    return NULL;
  }

  /* Lecture du fichier */
  if (fread(contents, sizeof(byte_t), size, file) != size) {
    return NULL;
  }

  /* Fermeture du fichier */
  if (fclose(file) != 0) {
    return NULL;
  }

  /* Stockage de la taille */
  if (p_size != NULL) {
    *p_size = size;
  }

  return contents;
}

int     contents_write( char *filename, byte_t *contents, size_t size ) {
  // TODO
  FILE *file = NULL;

  /* Ouverture du fichier */
  file = fopen(filename, "wb");

  /* Vérification de l'ouverture */
  if (file == NULL) {
    return 1;
  }

  /* Ecriture dans le fichier */
  if (fwrite(contents, sizeof(byte_t), size, file) != size) {
    return 2;
  }

  /* Fermeture du fichier */
  if (fclose(file) != 0) {
    return 3;
  }    

  return 0;
}
