#include "minmax.h"
#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Initialise une situation en fonction des arguments donnés. */
Situation nouvelle_situation(int *plateau, int joueur, int pts_j1, int pts_j2) {
  Situation s = {plateau, pts_j1, pts_j2, joueur};
  return s;
}

/* Fonction d'évaluation. */
int evaluation(Situation s, int joueur) {
  return joueur == 0 ? s.pts_j1 - s.pts_j2 : s.pts_j2 - s.pts_j1;
}

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
  int *cp = coups_possibles(s.plateau, joueur_a_maximiser);
  for (int i = 0; i < 12; i++) {
    // Regarder si case a 1 taille 12
    if (cp[i]) {

      if (i < 6 && joueur_a_maximiser == 0) {
        nouveau_fils(racine, i);
      } else {
        nouveau_fils(racine, i);
      }
    }
  }
  return racine;
}
