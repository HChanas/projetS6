#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NB_FILS_MAX T_PLAT / 2

#include "minimax.h"

/* Initialise une situation en fonction des arguments donnés. */
Situation nouvelle_situation(int *plateau, int joueur, int pts_j1, int pts_j2) {
  Situation s = {plateau, pts_j1, pts_j2, joueur};
  return s;
}

/* Fonction d'évaluation. */
int evaluation(Situation s, int joueur) {
  return joueur == 0 ? s.pts_j1 - s.pts_j2 : s.pts_j2 - s.pts_j1;
}

typedef struct Noeud Noeud;
struct Noeud {
  int valeur;  // Si le noeud est une feille ou si la valeur de ses fils a été
               // évaluée. Valeur donnée initialement aux feuilles par la
               // fonction d'évaluation.
  int feuille; // Indique si le noeud est une feuille pour simplifier des trucs
  int coups;
  int numero_joeur;
  Noeud *fils[NB_FILS_MAX];
  // Tableau de pointeurs vers les fils. Il peut y avoir
  // des NULL, si certains coups étaient impossibles.
  // A la i-ème case du tableau, se trouve l'adresse du noeud correspondant au
  // coup i(+1). e.g. fils[1] -> trou 2 (ou 8 si joueur 2). Si par exemple le
  // trou 4 (ou 10 donc) ne peut être joué, fils[3] = NULL.
};

/* Initialise les champs du noeud (fils à NULL et valeur à 0 par défaut). */
void init_noeud(Noeud *nouveau) {

  nouveau->valeur = 0;
  nouveau->feuille = 0;
  nouveau->coups = 0;
  nouveau->numero_joeur = 0;
  for (int i = 0; i < NB_FILS_MAX; i++) {
    nouveau->fils[i] = NULL;
  }
}
/* Crée un nouveau noeud (initialisé) et met son adresse dans la case du tableau
 * fils donnée. L'adresse du nouveau noeud est renvoyée. */
Noeud *nouveau_fils(Noeud *n, int indice) {
  Noeud *nouveau = malloc(sizeof(*nouveau));
  if (nouveau)
    init_noeud(nouveau);
  else
    fprintf(stderr, "Memory Allocation Failed");
  n->fils[indice] = nouveau;
  return nouveau;
}

/* Libère récursivement tout l'espace mémoire d'un arbre. */
void free_arbre(Noeud *racine) {
  if (racine == NULL)
    return;
  for (int i = 0; i < NB_FILS_MAX; i++)
    free_arbre(racine->fils[i]);
  free(racine);
}

/* Génère un arbre de possibilités à partir d'un plateau de jeu et du joueur qui
 * va jouer. La profondeur de l'arbre est précisée. */
Noeud *nouvel_arbre(Situation s, int joueur_a_maximiser, int profondeur) {
  Noeud *racine = malloc(sizeof(*racine));
  if (racine)
    init_noeud(racine);
  else
    fprintf(stderr, "Memory Allocation Failed");
  racine->numero_joeur = joueur_a_maximiser;
  if (profondeur == 0) {
    racine->feuille = 1;
    racine->valeur = evaluation(s, joueur_a_maximiser);
    return racine;
  }
  int *cp = coups_possibles(Situation->plateau, joueur_a_maximiser);
  for (int i = 0; 12; i++) {
    // Regarder si case a 1 taille 12
    if (cp[i]) {
    }
  }
  return racine;
}
