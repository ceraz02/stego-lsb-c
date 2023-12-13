#ifndef _stego_h_
#define _stego_h_

#include <stdlib.h>

#include <steg1ano/byte.h>
#include <steg1ano/image.h>

typedef struct {
  unsigned int      canary; /* Marque signalant un fichier caché */
  unsigned int    name_len; /* Longueur du nom du fichier caché  */
  unsigned int content_len; /* Nombre d'octets du fichier caché  */
} stego_header_t;

//////
// Le code de ces fonctions est dans les fichiers src/stego_decode.c et src/stego_encode.c
//////

/**
 * Décode un message de taille size,
 *  caché dans l'image img à partir du pixel de ligne *p_row et de colonne *p_col,
 *  et stocke ce message dans le tableau d'octets ptr.
 * Stocke la ligne et la colonne du prochain pixel disponible
 *  pour lire un autre bloc dans  *p_row  et *p_col
 *  (c'est à dire le premier pixel qui n'a PAS été utilisé pour décoder les
 *  size octets du bloc caché)
 * @PARAM ptr  : pointeur sur le tableau d'octets où stocker le bloc caché
 * @PARAM size : nombre d'octets à extraire (et taille du tableau d'octets ptr)
 * @PARAM img  : l'image dans laquelle extraire le bloc 
 * @PARAM p_row, pcol : *p_row et *p_col sont les lignes et colonnes du premier 
 *                      pixel de l'image à considérer pour extraire le bloc caché.
 *                      Ils sont mis à jour par la fonction après décodage
 *                      pour désigner le pixel suivant le dernier pixel 
 *                      utilisé par le bloc à décoder.
 * @PARAM p    : profondeur de bits (nombre de bits de poids faible utilisé
 *               par le message caché dans les pixels).
 * @RETURN : 0 si OK
 * Erreurs :
 *  - si pas assez de pixels restant pour retrouver les size octets
 *        supposés cachés dans l'image.
 *  - si b différent de 1, 2, 4 ou 8 (hors bonus) 
 */
int stego_decode_block( byte_t *ptr, size_t size, image_t img, int *p_row, int *p_col, int b );


/**
 * Encode le message stocké dans le tableau d'octets ptr, de taille size,
 *  dans l'image img, à partir du pixel de ligne *p_row et de colonne *p_col.
 * Stocke la ligne et la colonne du prochain pixel disponible pour
 *  écrire un autre bloc dans  *p_row  et *p_col
 *  (c'est à dire le premier pixel qui n'a PAS été utilisé pour encoder les
 *  size octets du message ptr)
 * @PARAM ptr  : pointeur sur le tableau d'octets contenant le bloc à encoder.
 * @PARAM size : ombre d'octets du bloc à encoder (<=> taille du tableau d'octets ptr)
 * @PARAM img  : l'image dans laquelle cacher le bloc 
 * @PARAM p_row, pcol : *p_row et *p_col sont les lignes et colonnes du premier 
 *                      pixel de l'image à considérer pour enfouir le bloc.
 *                      Ils sont mis à jour par la fonction après décodage
 *                      pour désigner le pixel suivant le dernier pixel 
 *                      utilisé par le bloc à enfouir.
 * @PARAM p    : profondeur de bits (nombre de bits de poids faible utilisé
 *               pour cacher le message dans les pixels).
 * @RETURN : 0 si OK.
 * Erreurs :
 *  - si pas assez de pixels restant pour cacher les size octets dans l'image.
 *  - si b différent de 1, 2, 4 ou 8 (hors bonus) 
 */
int stego_encode_block( byte_t *ptr, size_t size, image_t img, int *p_row, int *p_col, int b );


/**
 * Encode le contenu du fichier nommé filename dans l'image img,
 * en utilisant une profondeur de b bits, c'est à dire les trois blocs :
 *    Encode le Header (canari, taille du nom de fichier, taille du fichier)
 *    Encode le nom de fichier
 *    Encode le contenu du fichier.
 *
 * @RETURN : 0 si OK.
 * Erreurs :
 *    - si le fichier filename n'est pas lisible.
 *    - si pas assez de pixels dans l'image pour encoder les trois blocs.
 *    - si b différent de 1, 2, 4 ou 8 (hors bonus) 
 */
int stego_encode( image_t img, char *filename, int b );

/**
 * Analyse une image pour détecter la présence éventuelle du canari en testant toutes
 * les valeurs possibles de b.
 *
 * @RETURN : Retourne la valeur de b pour laquelle le canari est retrouvé, sinon -1.
 */
int stego_decode_analyse_canary( image_t img );

/**
 * Décodage d'une information cachée dans une image.
 * 
 * Utilise stego_decode_analyse_canary pour retrouver le 
 * nombre de bits utilisé.
 *
 * Si décodage possible : affiche le nom du fichier dont le nom
 * et le contenu sont cachés dans l'image et recrée ce fichier.
 *
 * @RETURN : 0 si un message était caché et que le fichier caché a pu être recréé
 * @RETURN : -1  si aucun message n'est caché dans l'image img
 * @RETURN : -2 si le fichier filename ne peut pas être écrit
 */
int stego_decode( image_t img );

#endif
