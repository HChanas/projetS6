#ifndef MINMAX_H
#define MINMAX_H

#include "jeu.h"

#define NB_FILS_MAX T_PLAT/2

#define MIN(x, y) ((x)>(y)?(y):(x))
#define MAX(x, y) (-MIN(-(x),-(y)))

typedef struct noeud_t{
    struct noeud_t* fils[NB_FILS_MAX]; //Tableau de pointeurs vers les fils. Il peut y avoir des NULL, si certains coups étaient impossibles.
    //A la i-ème case du tableau, se trouve l'adresse du noeud correspondant au coup i(+1). e.g. fils[1] -> trou 2 (ou 8 si joueur 2).
    //Si par exemple le trou 4 (ou 10 donc) ne peut être joué, fils[3] = NULL.
    int valeur;  // Si le noeud est une feille ou si la valeur de ses fils a été
               // évaluée. Valeur donnée initialement aux feuilles par la
               // fonction d'évaluation.
    int feuille; // Indique si le noeud est une feuille pour simplifier des trucs
    int coups;
    int numero_joeur;
} Noeud;

typedef struct situation_t{ //enregistre l'état d'une partie
    int* plateau;       //le plateau
    int pts_j1;         //points du joueur 1
    int pts_j2;         //joueur 2
    int joueur_tour;    //le joueur à qui c'est le tour de jouer et tout bref
} Situation;

/*--- FONCTION DE GESTION DE L'ARBRE ---*/

/* Initialise les champs du noeud (fils à NULL et valeur à 0 par défaut). */
void init_noeud(Noeud* n);

/* Crée un nouveau noeud (initialisé) et met son adresse dans la case du tableau fils donnée. L'adresse du nouveau noeud est renvoyée. */
Noeud* nouveau_fils(Noeud* n, int indice);

/* Libère récursivement tout l'espace mémoire d'un arbre. */
void free_arbre(Noeud* racine);

/*--- STRUCTURE SITUATION ---*/

/* Initialise une situation en fonction des arguments donnés. */
Situation nouvelle_situation(int* plateau, int joueur, int pts_j1, int pts_j2);

/* Copie d'une situation (notamment le plateau). Plateau à free ! */
Situation copie_situation(Situation s);

/*--- GENERATION DE L'ARBRE ---*/

/* Utilise les fonction du jeu pour calculer le plateau et les points des joueurs si le coup indiqué est joué.
 * Cette fonction ressemble donc à tour_de_jeu(), mais sans demander le coup dans le terminal. 
 * Il faut vérifier avant d'appeler cette fonction quels sont les coups possibles. */
void calcul_coup(Situation* s, int coup_joue);

/* Génère un arbre de possibilités à partir d'un plateau de jeu et du joueur qui va jouer.
 * La profondeur de l'arbre est précisée. */
Noeud* nouvel_arbre(Situation s, int joueur_a_maximiser, int profondeur);

/*--- FONCTION D'EVALUATION ---*/

/* Fonction simple qui renvoie une valeur en fonction des points des joueurs.
 * Ce nombre doit permettre d'évaluer si une situation est à l'avantage du joueur ou non.
 * Peut être : return joueur_a_maximiser==0 ? pts_j1-pts_j2 : pts_2-pts_j1 tout simplement (version simple) */
int evaluation(Situation s, int joueur_a_maximiser);

/*--- EVALUATION DE L'ARBRE ---*/

/* En fonction de la valeur des feuilles données par la fonction d'évaluation,
 * calcule la valeur de tous les noeuds. coup permet de récupérer le coup à jouer.
 * joueur_a_maximiser est à 1 de base, car la racine est un noeud qui correspond
 * au joueur que l'on cherche à maximiser. A chaque appel récursif joueur_a_maximier
 * varie entre 0 et 1. */
void eval_arbre(Noeud* racine, int joueur_a_maximiser, int* coup);

/// S'AFFRANCHIR DE L'ARBRE

/* Version sans contruction d'arbre, pour éviter de consommer de la mémoire. */
int minmax_leger(Situation s, int profondeur, int joueur_a_maximiser, int *coup);

/// ELAGAGE ALPHA-BETA

/* Minmax en appliquant la logique de l'élagage alpha-beta. */
void minmax_alphabeta(Situation s, int profondeur, int joueur_a_maximiser, int *coup, int *alpha, int *beta);

#endif