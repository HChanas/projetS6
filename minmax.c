#include "minmax.h"
#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Copie d'une situation (notamment le plateau). Plateau à free ! */
Situation copie_situation(Situation s){
    int* np = malloc(T_PLAT*sizeof(int));
    for(int i=0; i<T_PLAT; i++)
        np[i] = s.plateau[i];
    return (Situation) {np, s.pts_j1, s.pts_j2, s.joueur_tour};
}

void calcul_coup(Situation *s, int coup) {
  // Répartition du tas en sens horaire
  repartition(*s, &coup);
  // Tas de billes mangés
  captures(s, &coup);
}

/* Fonction d'évaluation. */
int evaluation(Situation s, int joueur) {
  return joueur == 0 ? s.pts_j1 - s.pts_j2 : s.pts_j2 - s.pts_j1;
}

/* Génère un arbre de possibilités à partir d'un plateau de jeu et du joueur qui va jouer.
 * La profondeur de l'arbre est précisée. */
Noeud* nouvel_arbre(Situation s, int joueur_a_max, int profondeur,int coup){
    Noeud* racine = malloc(sizeof(Noeud));
    for (int i = 0; i < NB_FILS_MAX; i++)
        racine->fils[i] = NULL;
    racine->feuille=0;
    racine->valeur=696969;
    racine->coups=coup;
    if(profondeur==0){
        racine->feuille = 1;
        racine->valeur = evaluation(s, joueur_a_max);
        return racine;
    }
    int* c_possibles = coups_possibles(s);
    for(int i=0; i<T_PLAT; i++){
        if(c_possibles[i]){
            Situation s2 = copie_situation(s);
            calcul_coup(&s2, i);
            s2.joueur_tour = 1 - s2.joueur_tour;
            racine->fils[i-s.joueur_tour*6] = nouvel_arbre(s2, joueur_a_max, profondeur-1,i);
            free(s2.plateau);
        }
    }
    free(c_possibles);
    return racine;
}

/* Libère récursivement tout l'espace mémoire d'un arbre. */
void free_arbre(Noeud *racine) {
  if (racine == NULL)
    return;
  for (int i = 0; i < NB_FILS_MAX; i++)
    free_arbre(racine->fils[i]);
  free(racine);
}

// afficher l'abre
void print_tree(Noeud *racine, int depth) {
  if (racine == NULL)
    return;
  for (int i = 0; i < depth; i++)
    printf("\t");
  printf("(%d) %d\n", racine->coups+1,racine->valeur);
  for (int i = 0; i < NB_FILS_MAX; i++)
    print_tree(racine->fils[i], depth + 1);
}

/* Fonction qui joue n coups de façon aléatoire sur une partie. */
void coups_aleatoires(Situation *s, int n) {
  for (int i = 0; i < n; i++) {
    int *cp = coups_possibles(*s);
    int ncp = nb_cp(cp, T_PLAT); // on compte les cases à 1
    int rd = rand() % ncp;       // un nombre entre 0 et le nombre de cases à 1
    int compteur = 0, j;
    for (j = 0; j < T_PLAT; j++) { // on fait correspondre le nombre tiré à
                                   // la bonne case du tableau
      if (cp[j])
        compteur++;
      if (compteur > rd)
        break;
    }
    repartition(*s, &j);
    captures(s, &j);
    s->joueur_tour = 1 - s->joueur_tour;
  }
}

/* Fonction qui lance k parties entre deux IA, utilisant les fonctions
 * d'évalutations ainsi que les profondeurs données. Les k/6 premiers coups
 * sont joués aléatoirement, et la fonction minmax_alphabeta est utilisée. Le
 * troisième argument prend un tableau de pointeurs de fonctions de taille 2
 * (donc les deux fonctions d'évalutations)*/

/* VERSION 'ALÉATOIRE' EN ATTENDANT QUE MINMAX ALPHA-BETA SOIR IMPLEMENTÉE */
Donnees affrontements_successifs(int k, int profondeurs[2],
                                 int (**eval)(Situation s,
                                              int joueur_a_maximiser)) {
  int nb_coups_alea = k / 6, res, /*alpha, beta, coup,*/ nb_v_j1 = 0,
      nb_v_j2 = 0, tt_pts_j1 = 0, tt_pts_j2 = 0;
  for (int i = 0; i < k; i++) { // k parties
    int plateau[T_PLAT] = {INIT_TAB};
    Situation s = {plateau, 0, 0, 0};
    coups_aleatoires(&s, nb_coups_alea);
    while (1) {
      /*minmax_alphabeta(s, profondeurs[s.joueur_tour], 1, &coup, &alpha,
       * &beta, eval[s.joueur_tour]);*/
      // calcul du prochain coup avec la fonction d'évaluation choisie pour ce
      // joueur
      coups_aleatoires(&s, 1);
      /*repartition(s, &coup);
      captures(&s, &coup);
      s.joueur_tour = 1 - s.joueur_tour;*/
      res = verif_fin(s);
      if (res == 0)
        continue;
      tt_pts_j1 += s.pts_j1;
      tt_pts_j2 += s.pts_j2;
      if (res == 1)
        nb_v_j1++;
      else if (res == 2)
        nb_v_j2++;
      break;
    }
  }
  return (Donnees){(float)nb_v_j1 / k, (float)nb_v_j2 / k, (float)tt_pts_j1 / k,
                   (float)tt_pts_j2 / k};
}
