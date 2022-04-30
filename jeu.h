#ifndef JEU_H
#define JEU_H

#define T_PLAT 12
#define INIT_TAB 4,4,4,4,4,4,4,4,4,4,4,4

typedef struct situation_t{ //enregistre l'état d'une partie
    int* plateau;       //le plateau
    int pts_j1;         //points du joueur 1
    int pts_j2;         //joueur 2
    int joueur_tour;    //le joueur à qui c'est le tour de jouer
} Situation;

/* Lance une partie entre deux joueurs humains (local). */
void partie_pvp();
/* Vérifie si la partie est finie.
 * 0 -> pas finie ; 1 -> joueur 1 a gagné ; 2 -> joueur 2 a gagné ; 3 -> égalité. */
int verif_fin(Situation s);
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
/* Vérifie si le coup 'affame' l'adversaire, ce qui signifie qu'on capture
 * toutes les graines du camp adverse. */
int affame(int* plateau, int trou);
/* Vérifie les tas capturés selon les règles et calcule les points gagnés. */
void captures(int* plateau, int* trou, int* pts_joueur, int joueur);
/* Dans quel camp est le trou (0 ou 1) -1 => trou invalide. */
int camp(int trou);
/* Vérifie si il y a au moins une bille dans le camp du joueur donné (renvoie 0, 1 sinon). */
int est_affame(int* plateau, int joueur);
/* Une règle stipule que si le joueur adverse est affamé (plus de graine dans son camp),
 * alors le joueur doit obligatoirement le nourrir (jouer un coup qui remet des graines).
 * Si le joueur ne peut nourrir son adversaire, alors il capture toutes les graines restantes et gagne.
 * 
 * Donne l'ensemble des coups possibles qui nourrissent l'adversaire parmi les coups possibles déjà calculés. */
void coups_nourrissants(int* plateau, int joueur, int* coups_possibles);
/* Tentative de détection des situations où le jeu dure indéfiniement...
 * Pour le moment se contente de vérifier si il reste plus que 2 graines dans le jeu.
 * Peut-être que c'est suffisant mais jsp ^^ xD
 * renvoie 1 si la partie va durer à l'infini. */
int verif_partie_infinie(Situation s);

#endif