#ifndef _byte_h_
#define _byte_h_

#include <stdlib.h>

typedef unsigned char byte_t;

//////
// Le code de ces fonctions est dans les fichiers 
//  src/byte_get.c, src/byte_set.c et src/byte_print.c
//////

/**
 * FONCTION FOURNIE
 * Fonction utilitaire pouvant être utile pour debugger : 
 * affiche tous les print_octets
 *  du bloc mémoire pointé par ptr, de taille size octets
 */
void print_octets(byte_t *ptr, size_t size) ; // fournie


/**
 * Stocke dans *p_lsb
 * les b bits de poids faible de byte.
 * @RETURN : 0 si OK.
 * Erreurs : 
 *  - si p_lsb == NULL
 *  - si b > 8
 * Remarque : "lsb" signifie "list significant bit"
 */
int byte_get_lsb( byte_t byte, unsigned int b, byte_t *p_lsb );

/**
 * Modifie les b bits de poids faible de *p_byte
 * en les remplaçant par les b bit de poids faible de val.
 * @RETURN : 0 si OK.
 * Erreurs : 
 *  - si p_byte == NULL
 *  - si b > 8
 */
int byte_set_lsb( byte_t *p_byte, unsigned int b, byte_t val );


/**
 * Stocke dans *p_res
 * les bits (firstbit) à (firstbit+b) de byte.
 *
 * Numérotation des bits : 
 *   Le bit 0 est le bit de poids faible. 
 *   Le bit 7 est le bit de poids fort.
 *
 * Exemples :
 *   Supposons que byte == 0x27
 *    c'est à dire les bits 00100111
 *   - si firstbit==0 et b==8
 *     alors *p_res est affecté de 0x27
 *   - si firstbit==0 et b==4
 *     alors *p_res est affecté de 0x07
 *   - si firstbit==4 et b==4
 *     alors *p_res est affecté de 0x02
 *   - si firstbit==2 et b==2
 *     alors *p_res est affecté de 0x01
 *   - si firstbit==6 et b==2
 *     alors *p_res est affecté de 0x00
 *   - si firstbit==0 et b==1
 *     alors *p_res est affecté de 0x01
 *   - si firstbit==4 et b==1
 *     alors *p_res est affecté de 0x00
 *
 * @RETURN : 0 si OK.
 * Erreurs : 
 *  - si p_res == NULL
 *  - si firstbit+b > 8
 */
int byte_get_bits( byte_t byte, unsigned int firstbit, unsigned int b, byte_t *p_res );


/**
 * Modifie les bits  (firstbit) à (firstbit+b) de *p_byte
 * en leur affectant les b de poids faible de val.
 *
 * Numérotation des bits : 
 *   Le bit 0 est le bit de poids faible. 
 *   Le bit 7 est le bit de poids fort.
 *
 * Exemples :
 *   Supposons que *p_byte == 0x27
 *    c'est à dire les bits 00100111
 *   et que val = 0x01.
 *   - si firstbit==0 et b==8
 *     alors *p_byte devient 0x01
 *   - si firstbit==0 et b==4
 *     alors *p_res est affecté de 0x21
 *   - si firstbit==4 et b==4
 *     alors *p_res est affecté de 0x17
 *   - si firstbit==2 et b==2
 *     alors *p_res est affecté de 0x2F
 *   - si firstbit==6 et b==2
 *     alors *p_res est affecté de 0xE7
 *   - si firstbit==1 et b==0
 *     alors *p_res est inchangé
 *   - si firstbit==1 et b==7
 *     alors *p_res st affecté de 0xA7
 *
 * @RETURN : 0 si OK.
 * Erreurs : 
 *  - si p_byte == NULL
 *  - si firstbit+b > 8
 */
int byte_set_bits( byte_t * p_byte, unsigned int firstbit, unsigned int b, byte_t val );




#endif
