#ifndef MINMAX_H
#define MINMAX_H

#include <string.h>
#include "jeu.h"

#define NB_FILS_MAX T_PLAT/2
#define INFINI 100

#define MIN(x, y) ((x)>(y)?(y):(x))
#define MAX(x, y) ((x)<(y)?(y):(x))

#define EN_SEC(x) ((double)(x)/CLOCKS_PER_SEC)
//permet de convertir un nombre de ticks de système en secondes

typedef struct noeud_t{
    struct noeud_t* fils[NB_FILS_MAX]; // Tableau de pointeurs vers les fils. Il peut y avoir des NULL, si certains coups étaient impossibles.
    // A la i-ème case du tableau, se trouve l'adresse du noeud correspondant au coup i(+1). e.g. fils[1] -> trou 2 (ou 8 si joueur 2).
    // Si par exemple le trou 4 (ou 10 donc) ne peut être joué, fils[3] = NULL.
    int valeur;     // Si le noeud est une feuille ou si la valeur de ses fils a été évaluée.
                    // Valeur donnée initialement aux feuilles par la fonction d'évaluation.
    int feuille;    // Indique si le noeud est une feuille pour simplifier les choses.
    int numero_joueur;  // Indique à quel joueur "appartient" le noeud.
} Noeud;

/*--- FONCTION DE GESTION DE L'ARBRE ---*/

/* Libère récursivement tout l'espace mémoire d'un arbre. */
void free_arbre(Noeud* racine);

/* affiche l'abre */
void print_tree(Noeud *racine, int depth);

/*--- STRUCTURE SITUATION ---*/

/* Copie d'une situation (notamment le plateau). Plateau à free ! */
Situation copie_situation(Situation s);

/*--- FONCTIONS D'EVALUATION ---*/

/* Fonction simple qui renvoie une valeur en fonction des points des joueurs.
 * Ce nombre doit permettre d'évaluer si une situation est à l'avantage du joueur ou non. */
int evaluation(Situation s, int joueur_a_maximiser);

/* Prend en compte la différence de points et la différence du nombre de graines dans le camp des joueurs. */
int eval_nb_graines(Situation s, int joueur_a_maximiser);

/* Compte le nombre de trous à zéro graines et améliore le résultat de eval_nb_grines. */
int eval_nb_graines_zeros(Situation s, int joueur_a_maximiser);

/*--- GENERATION DE L'ARBRE ---*/

/* Génère un arbre de possibilités à partir d'un plateau de jeu et du joueur qui va jouer.
 * La profondeur de l'arbre est précisée. */
Noeud* nouvel_arbre(Situation s, int joueur_a_max, int profondeur, int coup);

/*--- EVALUATION DE L'ARBRE ---*/

/* En fonction de la valeur des feuilles données par la fonction d'évaluation,
 * calcule la valeur de tous les noeuds. coup permet de récupérer le coup à jouer.
 * joueur_a_maximiser correspond au joueur que l'on cherche à maximiser, i.e. celui qu'on cherche à faire gagner. */
int eval_arbre(Noeud* racine, int joueur_a_maximiser, int* coup);


/// S'AFFRANCHIR DE L'ARBRE

/* Version sans contruction d'arbre, pour éviter de consommer de la mémoire. */
int minmax_leger(Situation s, int profondeur, int joueur_a_maximiser, int *coup);

/// ELAGAGE ALPHA-BETA

/* Minmax en appliquant la logique de l'élagage alpha-beta, ainsi que la simplification Negamax, qui transforme EN GROS les 'min' en '-max'.
 * La fonction d'évaluation à utiliser est donnée. */
int negamax_alphabeta(Situation s, int profondeur, int joueur_a_maximiser, int *coup, int alpha, int beta, int (*eval)(Situation,int));

/// COMPARER LES FONCTIONS D'EVALUATION

/* Données issues d'un duel acharné entre deux IA ayant des différences subtiles...
 * Le joueur 1 est toujours celui qui commence à jouer. */
typedef struct donnees_t{
    float taux_v1;  //pourcentage de victoire du joueur 1
    float taux_v2;  //pourcentage de victoire du joueur 2
    float moy_pts_j1;   //moyenne des points du joueur 1 à la fin de la partie
    float moy_pts_j2;   //moyenne des points du j2
} Donnees;

/* Fonction qui joue n coups de façon aléatoire sur une partie. srand à initialiser. */
void coups_aleatoires(Situation *s, int n);

/* Fonction qui lance k parties entre deux IA, utilisant les fonctions d'évalutations ainsi que les profondeurs données.
 * Les k/6+1 premiers coups sont joués aléatoirement, et la fonction minmax_alphabeta est utilisée.
 * Le troisième argument prend un tableau de pointeurs de fonctions de taille 2 (donc les deux fonctions d'évalutations)*/
Donnees affrontements_successifs(int k, int profondeurs[2], int (**eval)(Situation s, int joueur_a_maximiser));

/// COMPARER LES PERFORMANCES DES ALGORITHMES

/* Permet d'indiquer une fonction à utiliser. */
typedef enum fonction_a_utiliser{
    arbre,  //nouvel_arbre et eval_arbre
    leger,  //minmax_leger
    alphabeta   //negamax_alphabeta
} e_algo;

/* Pour l'affichage, modifie buf et le renvoie. */
char* e_algo_to_str(e_algo a, char* buf, size_t size);

/* Utilisée dans le main pour gérer les arguments du programme. */
e_algo str_to_e_algo(char* str);

/* Fonction qui joue k parties entre deux IA, comme affrontements_successifs,
 * mais mesure et affiche le temps total utilisé pour chaque IA à la fin.
 * algo_1 et algo_2 indiquent quelle fonction les IA utilisent. */
void test_de_performances(int k, int profondeur, e_algo algo_1, e_algo algo_2);

#endif