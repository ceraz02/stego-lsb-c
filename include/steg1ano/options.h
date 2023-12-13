#ifndef _options_h_
#define _options_h_

/**
 * Cherche l'option -letter=val dans la ligne de commande,
 * c'est à dire dans le tableau de chaine de caractères argv, de taille argc.
 *
 * @PARAM : letter : le code (lettre) de l'option recherchée
 *
 * @RETURN : NULL si l'option ne figure dans aucun des argv[i], 1 <= i < argc,
 * ou l'adresse du caractère suivant letter, ou encore du suivant si c'est un '='.
 */
char *option_string( char letter, int argc, char *argv[] );

/**
 * Cherche l'option de nom letter, supposée entière, dans le tableau argv
 * et si trouvée stocke *p_val la valeur entière de cette option
 * Exemple
 *    Si on cherche l'option 'b'  (letter vaut 'b')
 *    et que l'utilisateur a tappé -b=5 , alors stocke 5 dans *p_val
 *
 * @RETURN : 0 si l'option letter a été trouvée et correspond bien
 *           à une valeur entière.
 * Erreurs :
 *    - si p_val vaut NULL
 *    - si l'option cherchée n'est pas trouvée
 *    - si l'option n'a pas de valeur entière (eg -b=   ou -b=toto   ou  -b)
 */
int   option_int( char letter, int argc, char *argv[], int *p_val );

/**
 * Cherche l'option de nom letter dans le tableau argv
 *
 * @RETURN : 0 (false) si l'option letter n'a PAS été trouvée, 1 si trouvée
 */
int   option_flag( char letter, int argc, char *argv[] );

#endif
