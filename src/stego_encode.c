#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <steg1ano/byte.h>
#include <steg1ano/contents.h>
#include <steg1ano/stego.h>

#define CANARY 0xefbeadde




int stego_encode_block( byte_t *ptr, size_t size, image_t img, int *p_row, int *p_col, int b ) {
  // TODO

  // Vérification des paramètres
  if (ptr == NULL || img == NULL || p_row == NULL || p_col == NULL ) {
    return -1;
  }

  // Vérification de la profondeur de bits
  if (b != 1 && b != 2 && b != 4 && b != 8) {
    return -1;
  }

  // Vérification de pixels restants pour encoder le message de taille size
  int nb_pixels = img->width * img->height;
  int nb_pixels_restants = nb_pixels - (*p_row * img->width + *p_col);
  int nb_pixels_necessaires = size * 8 / b;
  if (nb_pixels_restants < nb_pixels_necessaires) {
    return -1;
  }

  // Encode le message dans l'image
  int i;
  // Boucle sur les octets du message
  for (i = 0; i < size; i++) {
    // Boucle sur les bits de l'octet
    int j = 0;
    for (j = 0; j < 8; j += b) {
      // Récupération du pixel
      byte_t pixel = 0;
      int ret = image_get_pixel(img, *p_row, *p_col, &pixel);
      if (ret != 0) {
        return -1;
      }

      // Récupération des b bits de poids faible de l'octet
      byte_t lsb = 0;
      ret = byte_get_lsb(ptr[i], b, &lsb);
      if (ret != 0) {
        return -1;
      }

      // Modification des b bits de poids faible du pixel
      ret = byte_set_lsb(&pixel, b, lsb);
      if (ret != 0) {
        return -1;
      }

      // Modification du pixel dans l'image
      ret = image_set_pixel(img, *p_row, *p_col, pixel);
      if (ret != 0) {
        return -1;
      }

      // Mise à jour de la position du pixel
      // printf("row : %d, col : %d\n", *p_row, *p_col);
      if (*p_col == img->width - 1) {
        *p_col = 0;
        *p_row += 1;
      } else {
        *p_col += 1;
      }
    }
  }

  return 0;
}

int stego_encode( image_t img, char *filename, int b ) {
  // TODO
}

