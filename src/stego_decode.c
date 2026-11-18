#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <steg1ano/byte.h>
#include <steg1ano/contents.h>
#include <steg1ano/stego.h>

#define CANARY 0xefbeadde



int stego_decode_block( byte_t *ptr, size_t size, image_t img, int *p_row, int *p_col, int b ) {
  // Vérification de la valeur de b
  if(!(1==b || 2==b || 4==b || 8==b)) {
    fprintf(stderr, "ERROR stego_decode_block : b différent de 1, 2, 4 ou 8.\n");
    return -1;
  }

  byte_t byteCache; // Variable qui va contenir les bits de poids faible du pixel
  int nb_pixel = 8/b; // Nombre de pixels nécessaires pour encoder un octet

  // On décode chaque octet du message dans l'image
  for(size_t oct=0; oct<size; oct++) {
    // On décode chaque bit de l'octet dans un pixel
    for(int i=0; i<nb_pixel; i++) {
      // On vérifie, avant de lire, qu'il reste un pixel disponible
      if(*p_row>=img->height) {
        fprintf(stderr, "ERROR stego_decode_block : pas assez des pixels restants.\n");
        return -1;
      }

      // On récupère les bits de poids faible du pixel dans byteCache
      byte_get_lsb(img->pix[*p_row][*p_col], b, &byteCache);
      // On met les bits de poids faible du pixel à la valeur de byteCache
      byte_set_bits(ptr, i*b, b, byteCache);

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

int stego_decode_analyse_canary( image_t img ) {
  // Tableau des valeurs de b
  char b_list[] = {1, 2, 4, 8};
  int row, col;
  unsigned int canary;
  byte_t *pcanary = (byte_t*)&canary; // Pointeur sur le canary

  // On décode le canary avec b=1, 2, 4 et 8
  for(int i=0; i<4; i++) {
    pcanary = (byte_t*)&canary;
    row = 0, col = 0;
    canary = 0;
    stego_decode_block(pcanary, 4, img, &row, &col, b_list[i]); // On décode le canary

    // On vérifie que le canary est bien égal à CANARY
    if(canary==CANARY) return b_list[i];
  }

  // Si on arrive ici, c'est que le canary n'a pas été trouvé
  return -1;
}

int stego_decode( image_t img ) {
  int b = stego_decode_analyse_canary(img);
  if(-1==b) {
    fprintf(stderr, "ERROR stego_decode : aucun message n'est caché dans l'image.\n");
    return -1;
  }
  int row = 0, col = 0;

  stego_header_t stegoHeader;
  if(stego_decode_block((byte_t *)&stegoHeader, sizeof(stegoHeader), img, &row, &col, b)) return -1;

  // +1 : le nom lu dans l'image doit se terminer par un caractère nul
  char *filename = calloc((size_t)stegoHeader.name_len + 1, 1);
  if(NULL==filename) return -1;
  if(stego_decode_block((byte_t *)filename, stegoHeader.name_len, img, &row, &col, b)) {
    free(filename);
    return -1;
  }

  byte_t *contents = calloc((size_t)stegoHeader.content_len + 1, 1);
  if(NULL==contents) {
    free(filename);
    return -1;
  }
  if(stego_decode_block(contents, stegoHeader.content_len, img, &row, &col, b)) {
    free(filename);
    free(contents);
    return -1;
  }

  if(contents_write(filename, contents, stegoHeader.content_len)!=0) {
    fprintf(stderr, "ERROR stego_decode : le fichier %s ne peut pas être écrit.\n", filename);
    free(filename);
    free(contents);
    return -2;
  }

  free(filename);
  free(contents);

  return 0;
}
