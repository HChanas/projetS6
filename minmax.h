#include "jeu.h"

#define NB_FILS_MAX T_PLAT/2

#define MIN(x, y) ((x)>(y)?(y):(x))
#define MAX(x, y) (-MIN(-(x),-(y)))

typedef struct noeud_t{
    noeud_t* fils[NB_FILS_MAX]; //Tableau de pointeurs vers les fils. Il peut y avoir des NULL, si certains coups étaient impossibles.
    //A la i-ème case du tableau, se trouve l'adresse du noeud correspondant au coup i(+1). e.g. fils[1] -> trou 2 (ou 8 si joueur 2).
    //Si par exemple le trou 4 (ou 10 donc) ne peut être joué, fils[3] = NULL.
    int valeur; //Si le noeud est une feille ou si la valeur de ses fils a été évaluée. Valeur donnée initialement aux feuilles par la fonction d'évaluation.
    int feuille; //Indique si le noeud est une feuille pour simplifier des trucs
} Noeud;

/*--- FONCTION DE GESTION DE L'ARBRE ---*/

/* Initialise les champs du noeud (fils à NULL et valeur à 0 par défaut). */
void init_noeud(Noeud* n);

/* Crée un nouveau noeud (initialisé) et met son adresse dans la case du tableau fils donnée. L'adresse du nouveau noeud est renvoyée. */
Noeud* nouveau_fils(Noeud* n, int indice);

/* Libère récursivement tout l'espace mémoire d'un arbre. */
void free_arbre(Noeud* racine);

/*--- GENERATION DE L'ARBRE ---*/

/* Utilise les fonction du jeu pour calculer le plateau et les points des joueurs si le coup indiqué est joué.
 * Cette fonction ressemble donc à tour_de_jeu(), mais sans demander le coup dans le terminal. 
 * Il faut vérifier avant d'appeler cette fonction quels sont les coups possibles. */
void calcul_coup(int* plateau, int* pts_j1, int* pts_j2, int coup_joue);

/* Génère un arbre de possibilités à partir d'un plateau de jeu et du joueur qui va jouer.
 * La profondeur de l'arbre est précisée. */
Noeud* nouvel_arbre(int* plateau, int joueur, int pts_j1, int pts_j2, int profondeur);

/*--- FONCTION D'EVALUATION ---*/

/* Fonction simple qui renvoie une valeur en fonction des points des joueurs.
 * Ce nombre doit permettre d'évaluer si une situation est à l'avantage du joueur ou non.
 * Peut être : return joueur==1 ? pts_j1-pts_j2 : pts_2-pts_j1 tout simplement */
int evaluation(int pts_j1, int pts_j2, int joueur/*, autres ?*/);

/*--- EVALUATION DE L'ARBRE ---*/

/* En fonction de la valeur des feuilles données par la fonction d'évaluation,
 * calcule la valeur de tous les noeuds.*/
void eval_arbre(Noeud* racine);

/* Evalue l'arbre en appliquant la logique de l'élagage alpha-beta. */
void eval_arbre_alphabeta(Noeud* racine, int alpha, int beta);
// Peut être implémenté après avoir fait marcher l'algo de base. On pourra comparer les perfs et les résultats des deux.