#ifndef _image_h_
#define _image_h_

typedef unsigned char pixel_t;

typedef struct {
  int      height; /* Nombre de lignes   : M       */
  int       width; /* Nombre de colonnes : N       */
  int      nbchan; /* Nombre de canaux couleur     */
  pixel_t   **pix; /* Tableau des lignes de pixels */
} *image_t;


/**
 * Alloue une image de taille width*height pixels
 * avec nbchan canaux colorimétriques
 * et initialise les pixels avec des zéros.
 *
 * Le tableau bidimensionnel de pixels doit être une représentation
 * contiguë en mémoire.
 *
 * @RETURN : l'adresse de l'image en mémoire, ou NULL si erreur.
 * Erreurs : 
 *   - Si height ou width sont < 0. 
 *   - Si chan est différent de 0 ou 3.
 */
image_t  image_new( int height, int width, int nbchan );

/**
 * Libère une image préalablement allouée avec image_new().
 * @RETURN : 0 si OK
 * Erreurs : si le pointeur img vaut NULL
 */
int     image_delete( image_t img );


/**
 * Affiche l'image img dans le terminal
 * @RETURN un nombre négatif si img == NULL, >=0 sinon
 */
int      image_printf( image_t img ); // fournie

/**
 * Retourne la hauteur de l'image img (nombre de lignes)
 * ou 0 si le pointeur img vaut NULL
 */
int      image_get_height( image_t img );

/**
 * Retourne la largeur de l'image img (nombre de colonnes)
 * ou 0 si le pointeur img vaut NULL
 */
int      image_get_width( image_t img );

/**
 * Retourne le nombre de canaux colorimétriques de l'image img
 * ou 0 si le pointeur img vaut NULL
 */
int      image_get_nb_chan( image_t img );

/**
 * Stocke dans *p_pix le pixel situé à la ligne r et la colonne c de l'image img
 * img doit être être une image en niveau de gris.
 * @RETURN : 0 si OK, -1 en cas d'erreur.
 * Erreurs :
 *  - si r n'est pas une ligne valide ou c n'est pas une colonne valide de l'image
 *  - si img vaut NULL
 */
int      image_get_pixel( image_t img, int r, int c, pixel_t *p_pix );

/**
 * Modifie le pixel à la ligne r et la colonne c de l'image img à la valeur pixel.
 * img doit être être une image en niveau de gris.
 * @RETURN : 0 si OK, -1 en cas d'erreur.
 * Erreurs :
 *  - si r n'est pas une ligne valide ou c n'est pas une colonne valide de l'image
 *  - si img vaut NULL
 */
int      image_set_pixel( image_t img, int r, int c, pixel_t pix );

#endif
