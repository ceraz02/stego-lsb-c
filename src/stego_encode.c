#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <steg1ano/byte.h>
#include <steg1ano/contents.h>
#include <steg1ano/stego.h>

#define CANARY 0xefbeadde




int stego_encode_block( byte_t *ptr, size_t size, image_t img, int *p_row, int *p_col, int b ) {
  // Vérification de la valeur de b
  if(!(1==b || 2==b || 4==b || 8==b)) {
    fprintf(stderr, "ERROR stego_encode_block : b différent de 1, 2, 4 ou 8.\n");
    return -1;
  }

  byte_t byteCache; // Variable qui va contenir les bits de poids faible du pixel
  int nb_pixel = 8/b; // Nombre de pixels nécessaires pour encoder un octet

  // On encode chaque octet du message dans l'image
  for(size_t oct=0; oct<size; oct++) {
    // On encode chaque bit de l'octet dans un pixel
    for(int i=0; i<nb_pixel; i++) {
      // On vérifie, avant d'écrire, qu'il reste un pixel disponible
      if(*p_row>=img->height) {
        fprintf(stderr, "ERROR stego_encode_block : pas assez des pixels restants.\n");
        return -1;
      }

      // On récupère les bits de poids faible du pixel dans byteCache
      byte_get_bits(*ptr, i*b, b, &byteCache);
      // On met les bits de poids faible du pixel à la valeur de byteCache
      byte_set_lsb(&(img->pix[*p_row][*p_col]), b, byteCache);

      // On passe au pixel suivant
      if(*p_col<img->width-1) {
        (*p_col)++;
      }
      else {
        (*p_row)++;
        *p_col = 0;
      }
    }
    ptr++;
  }
  return 0;
}

int stego_encode( image_t img, char *filename, int b ) {
  if(!(1==b || 2==b || 4==b || 8==b)) {
    fprintf(stderr, "ERROR stego_encode : b différent de 1, 2, 4 ou 8.\n");
    return -1;
  }

  size_t content_len = 0;
  byte_t *contents = contents_read(filename, &content_len);
  if(NULL==contents) {
    fprintf(stderr, "ERROR stego_encode : le fichier %s ne peut pas être lu.\n", filename);
    return -1;
  }

  stego_header_t stegoHeader;
  stegoHeader.canary = CANARY;
  stegoHeader.name_len = strlen(filename);
  stegoHeader.content_len = content_len;

  int row = 0, col = 0;

  if(stego_encode_block((byte_t *)&stegoHeader, sizeof(stegoHeader), img, &row, &col, b)) {
    fprintf(stderr, "ERROR stego_encode : Encodage de Header échoue.\n");
    free(contents);
    return -1;
  }
  if(stego_encode_block((byte_t *)filename, stegoHeader.name_len, img, &row, &col, b)) {
    fprintf(stderr, "ERROR stego_encode : Encodage de nom du fichier %s échoue.\n", filename);
    free(contents);
    return -1;
  }
  if(stego_encode_block(contents, stegoHeader.content_len, img, &row, &col, b)) {
    fprintf(stderr, "ERROR stego_encode : Encodage de contenu du fichier %s échoue.\n", filename);
    free(contents);
    return -1;
  }

  free(contents);
  return 0;
}
