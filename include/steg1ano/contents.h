#ifndef _contents_h_
#define _contents_h_

#include <stdlib.h>

#include <steg1ano/byte.h>

/*
 * Charge en mémoire le contenu du fichier nommé  filename
 * dans un tableau d'octets alloué dynamiquement,
 * et stocke la taille du fichier (i.e. taille du tableau d'octet) dans *p_size.
 *
 * Le fichier sera considéré comme un fichier binaire 
 * (pour lire les octets à l'identique), même si il contient
 * du code ASCII.
 *
 * Le tableau d'octets dont l'adresse est retournée sera alloué dynamiquement.
 * Il devra donc être ultérieurement être libéré avec free(3).
 *
 * @RETURN : l'adresse du tableau d'octet lu dans le fichier, 
 *            ou NULL en cas d'erreur.
 * Erreurs :
 *    - si le fichier filename est illisible.
 *    - si p_size vaut NULL
 */
byte_t *contents_read( char *filename, size_t * p_size );


/*
 * Ecrit le tableau d'octets contents de taille size
 * dans un fichier nommé filename.
 *
 * Le fichier sera considéré comme un fichier binaire 
 * (pour lire les octets à l'identique), même si il contient
 * du code ASCII.
 *
 * @RETURN : 0 si OK.
 *
 * Erreurs :
 *    - si le fichier filename ne peut être ouvert en écriture, 
 *      ou en cas d'erreur d'écriture.
 */
int     contents_write( char *filename, byte_t *contents, size_t size );

#endif
