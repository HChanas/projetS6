#define T_PLAT 12
#define INIT_TAB 4,4,4,4,4,4,4,4,4,4,4,4

/* Affiche le plateau et le score des joueurs. */
void affiche_jeu(int* plateau, int pts_j1, int pts_j2);
/* Affiche les trous du plateau de jeu. */
void affiche_plateau(int* plateau);
/* Demande une trou au joueur, effectue la répartition du tas choisi, 
 * gère le mangeage de billes si besoin et calcule les points. */
void tour_de_jeu(int* plateau, int joueur, int* pts_joueur);
/* Vérifie en fonction du tableau de coups possibles si l'entrée en est. */
int trou_valide(int* cp, int entree);
/* Calcule les coups possibles pour le tour d'un joueur et renvoie
 * un tableau de la taille du plateau avec les trous possibles à 1. */
int* coups_possibles(int* plateau, int joueur);
/* Réparti les billes du tas choisi dans les tas suivants selon le
 * principe de l'awalé. */
void repartition(int* plateau, int* trou);
/* Vérifie les tas capturés selon les règles et calcule les points gagnés. */
void captures(int* plateau, int* trou, int* pts_joueur, int joueur);
/* Dans quel camp est le trou (0 ou 1) -1 => trou invalide. */
int camp(int trou);