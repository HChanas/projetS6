#include "minmax.h"
#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*--- FONCTION DE GESTION DE L'ARBRE ---*/

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

/*--- STRUCTURE SITUATION ---*/

/* Copie d'une situation (notamment le plateau). Plateau à free ! */
Situation copie_situation(Situation s){
    int* np = malloc(T_PLAT*sizeof(int));
    for(int i=0; i<T_PLAT; i++)
        np[i] = s.plateau[i];
    return (Situation) {np, s.pts_j1, s.pts_j2, s.joueur_tour, s.nb_coups};
}

/*--- GENERATION DE L'ARBRE ---*/

/* Utilise les fonction du jeu pour calculer le plateau et les points des joueurs si le coup indiqué est joué.
 * Cette fonction ressemble donc à tour_de_jeu(), mais sans demander le coup dans le terminal. 
 * Il faut vérifier avant d'appeler cette fonction quels sont les coups possibles. */
void calcul_coup(Situation *s, int coup) {
  // Répartition du tas en sens horaire
  repartition(*s, &coup);
  // Tas de billes mangés
  captures(s, &coup);
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

/*--- FONCTIONS D'EVALUATION ---*/

/* Fonction d'évaluation. */
int evaluation(Situation s, int joueur) {
  return joueur == 0 ? s.pts_j1 - s.pts_j2 : s.pts_j2 - s.pts_j1;
}


/// ELAGAGE ALPHA-BETA

/* Minmax en appliquant la logique de l'élagage alpha-beta. La fonction d'évaluation à utiliser est donnée. */
int minmax_alphabeta(Situation s, int profondeur, int joueur_a_maximiser, int *coup, int alpha, int beta, int (*eval)(Situation,int)){
    if(profondeur==0)
        return(eval(s, joueur_a_maximiser));    //une feuille : on calcule l'évaluation
    int value = (joueur_a_maximiser==s.joueur_tour)?-50:50, m, c=0; //value = +l'infini ou -l'infini, +50 ou -50 car l'évalutaion ne peut pas renvoyer plus que ça
    int* c_possibles = coups_possibles(s);  //tous les coups que le joueur peut jouer
    for(int i=0; i<T_PLAT; i++){
        if(c_possibles[i]){
            Situation s2 = copie_situation(s);  //on copie la situation pour ne pas toucher à la situation donnée pour cet appel de la fonction
            calcul_coup(&s2, i);    //on calcul le coup joué sur la copie de la situation initiale
            s2.joueur_tour = 1 - s2.joueur_tour;    //après que le coup ait été joué, c'est au tour de l'adversaire
            m = minmax_alphabeta(s2, profondeur-1, joueur_a_maximiser, coup, alpha, beta, eval);    //évaluation du coup qui vient d'être joué
            if(joueur_a_maximiser==s.joueur_tour){  //noeud max
                if(m>value){    
                    c = i;
                    value = m;
                }
                //revient à faire value = max(value, minmax(fils, ...)), mais en gardant i en mémoire en bonus

                if(value >= beta) { //coupure beta
                    free(s2.plateau);
                    goto ret;
                }
                alpha = MAX(alpha,value);
            }
            else{
                if(m<value){ //min(value, m)
                    c = i;
                    value = m;
                }
                if(alpha >= value) { //coupure alpha
                    free(s2.plateau);
                    goto ret;
                }
                beta = MIN(beta,value);
            }
            free(s2.plateau);
        }
    }
    ret:
    free(c_possibles);
    *coup = c;
    return value;
}

/// COMPARER LES FONCTIONS D'EVALUATION

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
    free(cp);
    repartition(*s, &j);
    captures(s, &j);
    s->joueur_tour = 1 - s->joueur_tour;
  }
}

/* Fonction qui lance k parties entre deux IA, utilisant les fonctions
 * d'évalutations ainsi que les profondeurs données. Les log6(k)+1 premiers coups
 * sont joués aléatoirement, et la fonction minmax_alphabeta est utilisée. Le
 * troisième argument prend un tableau de pointeurs de fonctions de taille 2
 * (donc les deux fonctions d'évalutations)*/
Donnees affrontements_successifs(int k, int profondeurs[2], int (**eval)(Situation,int)) {
  int nb_coups_alea = (int)(log((double)k)/log(6.0)) + 1, res, coup, nb_v_j1 = 0, nb_v_j2 = 0, tt_pts_j1 = 0, tt_pts_j2 = 0;
  //                    calcul de log6(k)
    printf("%d\n",nb_coups_alea);
  for (int i = 0; i < k; i++) { // k parties
    int plateau[T_PLAT] = {INIT_TAB};
    Situation s = {plateau, 0, 0, 0, 0};
    coups_aleatoires(&s, nb_coups_alea);
    //si on commence du même point à chaque fois les parties seront strictement identiques, donc on fais des coups aléatoires
    while (1) {
      minmax_alphabeta(s, profondeurs[s.joueur_tour], s.joueur_tour, &coup, -50, 50, eval[s.joueur_tour]);
      // calcul du prochain coup avec la fonction d'évaluation choisie pour ce joueur
      repartition(s, &coup);
      captures(&s, &coup);
      s.joueur_tour = 1 - s.joueur_tour;
      s.nb_coups++;
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
  return (Donnees){(float)nb_v_j1 / k, (float)nb_v_j2 / k, (float)tt_pts_j1 / k,(float)tt_pts_j2 / k};
}
