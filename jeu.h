#define T_PLAT 12
#define INIT_TAB 4,4,4,4,4,4,4,4,4,4,4,4

/* Affiche le plateau et le score des joueurs. */
void affiche_jeu(int* plateau, int pts_j1, int pts_j2);
/* Affiche les cases du plateau de jeu. */
void affiche_plateau(int* plateau);
/* Demande une case au joueur, effectue la répartition du tas choisi, 
 * gère le mangeage de billes si besoin et calcule les points. */
void tour_de_jeu(int* plateau, int joueur, int* pts_joueur);
/* Calcule les coups possibles pour le tour d'un joueur et renvoie
 * un tableau de la taille du plateau avec les cases possibles à 1. */
int* coups_possibles(int* plateau, int joueur);