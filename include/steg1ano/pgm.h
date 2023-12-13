#ifndef _pgm_h_
#define _pgm_h_

#include <steg1ano/image.h>


/**
 * Charge une image PGM en mémoire
 * depuis le fichier nommé filename au format PGM
 * @RETURN : l'image (allouée avec image_new()), ou NULL si erreur
 * Erreurs :
 *  - si le fichier filename ne peut être lu
 *  - si le fichier filename ne respecte pas le format PGM en niveau de gris
 *  - si le pointeur img vaut NULL
 */
image_t pgm_read( char *filename );

/**
 * FONCTION FOURNIE
 * Ecrit l'image img dans le fichier nommé filename, au format PGM.
 * @RETURN : -1 en cas d'erreur ; le nombre de valeur écrites dans le fichier sinon.
 * Erreurs :
 *  - si le fichier nommé filename ne peut être écrit
 *  - si le pointeur img vaut NULL
 */
int     pgm_write( char *filename, image_t img );

#endif
