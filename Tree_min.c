#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NB_FILS_MAX T_PLAT / 2

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
