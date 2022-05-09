#include "minmax.h"
#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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
    printf("%d\n", racine->valeur);
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

/*--- FONCTIONS D'EVALUATION ---*/

/* Fonction d'évaluation. */
int evaluation(Situation s, int joueur) {
  return joueur == 0 ? s.pts_j1 - s.pts_j2 : s.pts_j2 - s.pts_j1;
}

/*--- GENERATION DE L'ARBRE ---*/

/* Génère un arbre de possibilités à partir d'un plateau de jeu et du joueur qui va jouer.
 * La profondeur de l'arbre est précisée. */
Noeud* nouvel_arbre(Situation s, int joueur_a_max, int profondeur,int coup){
    Noeud* racine = malloc(sizeof(Noeud));
    for (int i = 0; i < NB_FILS_MAX; i++)
        racine->fils[i] = NULL;
    racine->feuille=0;
    racine->valeur=696969;
    racine->numero_joueur = s.joueur_tour;
    int* c_possibles = coups_possibles(s);
    if((profondeur==0)||(nb_cp(c_possibles, T_PLAT)==0)){
        racine->feuille = 1;
        racine->valeur = evaluation(s, joueur_a_max);
        free(c_possibles);
        return racine;
    }
    for(int i=0; i<T_PLAT; i++){
        if(c_possibles[i]){
            Situation s2 = copie_situation(s);
            calcul_coup(&s2, i);
            racine->fils[i-s.joueur_tour*6] = nouvel_arbre(s2, joueur_a_max, profondeur-1,i);
            free(s2.plateau);
        }
    }
    free(c_possibles);
    return racine;
}

/*--- EVALUATION DE L'ARBRE ---*/

/* En fonction de la valeur des feuilles données par la fonction d'évaluation,
 * calcule la valeur de tous les noeuds. coup permet de récupérer le coup à jouer.
 * joueur_a_maximiser correspond au joueur que l'on cherche à maximiser, i.e. celui qu'on cherche à faire gagner. */
int eval_arbre(Noeud* racine, int joueur_a_maximiser, int* coup) {
    int val_min_max = racine->numero_joueur==joueur_a_maximiser?-INFINI:INFINI;  
    int buff = val_min_max; 
    int tmp  = 0; 
    if (racine == NULL )
        return val_min_max;
    if (racine->feuille == 1) {
        return racine->valeur;
    }
    for (int i = 0; i < NB_FILS_MAX; i++){
        if (racine->fils[i] == NULL )
            continue;
        if (racine->numero_joueur == joueur_a_maximiser) {
        // prendre le max 
            val_min_max = MAX(eval_arbre(racine->fils[i],joueur_a_maximiser,coup),val_min_max); 
            // si la valeur change je veux le savoir pour recuperer l'indice du coup 
            if (buff != val_min_max) {
                buff  = val_min_max; 
                tmp = i; 
            }                 
        }
        // prendre le min
        else { 
            val_min_max = MIN(eval_arbre(racine->fils[i],joueur_a_maximiser,coup),val_min_max); 
            // si la valeur change je veux le savoir pour recuperer l'indice du coup 
            if (buff != val_min_max) {
                buff  = val_min_max; 
                tmp = i;
            }                 
        }
    }
    *coup = tmp+6*joueur_a_maximiser; 
    return val_min_max; 
}

/// S'AFFRANCHIR DE L'ARBRE

/* Version sans contruction d'arbre, pour éviter de consommer de la mémoire. */
int minmax_leger(Situation s, int profondeur, int joueur_a_maximiser, int *coup){
    int* c_possibles = coups_possibles(s);
    if((profondeur==0)||(nb_cp(c_possibles, T_PLAT)==0)){
        free(c_possibles);
        return(evaluation(s, joueur_a_maximiser));
    }
    int value = (joueur_a_maximiser==s.joueur_tour)?-INFINI:INFINI, m, c=0;
    for(int i=0; i<T_PLAT; i++){
        if(c_possibles[i]){
            Situation s2 = copie_situation(s);
            calcul_coup(&s2, i);
            m = minmax_leger(s2, profondeur-1, joueur_a_maximiser, coup);
            int cond = joueur_a_maximiser==s.joueur_tour;
            //le joueur de notre noeud est-il celui que l'on veut avantager ?
            if((cond&&(m>value))||((!cond)&&(m<value))){
                c = i;
                value = m;
            }
            free(s2.plateau);
        }
    }
    free(c_possibles);
    *coup = c;
    return value;
}

/// ELAGAGE ALPHA-BETA

/* Minmax en appliquant la logique de l'élagage alpha-beta, ainsi que la simplification Negamax, qui transforme EN GROS les 'min(x,y)' en '-max(-x,-y)'.
 * La fonction d'évaluation à utiliser est donnée. */
int negamax_alphabeta(Situation s, int profondeur, int joueur_a_maximiser, int *coup, int alpha, int beta, int (*eval)(Situation,int)){
    int* c_possibles = coups_possibles(s);  //tous les coups que le joueur peut jouer
    if((profondeur==0)||(nb_cp(c_possibles, T_PLAT)==0)){
        free(c_possibles);
        return (s.joueur_tour==joueur_a_maximiser)?eval(s, joueur_a_maximiser):-eval(s, joueur_a_maximiser);
    }    //une feuille : on calcule l'évaluation
    int valeur = -INFINI, coup_tmp=0, res;
    for(int i=0; i<T_PLAT; i++){
        if(c_possibles[i]){
            Situation s2 = copie_situation(s);  //on copie la situation pour ne pas toucher à la situation donnée pour cet appel de la fonction
            calcul_coup(&s2, i);    //on calcul le coup joué sur la copie de la situation initiale
            res = -negamax_alphabeta(s2, profondeur-1, joueur_a_maximiser, coup, -beta, -alpha, eval);
            if(res>valeur){
                valeur=res;
                coup_tmp = i;
            }
            alpha = MAX(alpha, valeur);
            if(alpha >= beta){
                free(s2.plateau);
                goto ret;
            }
            free(s2.plateau);
        }
    }
    ret:
    free(c_possibles);
    *coup = coup_tmp;
    return valeur;
}

/// COMPARER LES FONCTIONS D'EVALUATION

/* Fonction qui joue n coups de façon aléatoire sur une partie. */
void coups_aleatoires(Situation *s, int n) {
    for (int i = 0; i < n; i++) {
        int *cp = coups_possibles(*s);
        int ncp = nb_cp(cp, T_PLAT); // on compte les cases à 1
        int rd = rand() % ncp;       // un nombre entre 0 et le nombre de cases à 1
        int compteur = 0, j;
        for (j = 0; j < T_PLAT; j++) { // on fait correspondre le nombre tiré à la bonne case du tableau
            if (cp[j])
                compteur++;
            if (compteur > rd)
                break;
        }
        free(cp);
        calcul_coup(s, j);
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
    for (int i = 0; i < k; i++) { // k parties
        int plateau[T_PLAT] = {INIT_TAB};
        Situation s = {plateau, 0, 0, 0, 0};
        coups_aleatoires(&s, nb_coups_alea);
        //si on commence du même point à chaque fois les parties seront strictement identiques, donc on fais des coups aléatoires
        while (1) {
            negamax_alphabeta(s, profondeurs[s.joueur_tour], s.joueur_tour, &coup, -INFINI, INFINI, eval[s.joueur_tour]);
            // calcul du prochain coup avec la fonction d'évaluation choisie pour ce joueur
            calcul_coup(&s, coup);
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

/// COMPARER LES PERFORMANCES DES ALGORITHMES

/* Pour l'affichage, modifie buf et le renvoie. */
char* e_algo_to_str(e_algo a, char* buf, size_t size){
    switch (a){
        case arbre: strncpy(buf, "(minmax arbre)", size); break;
        case leger: strncpy(buf, "(minmax leger)", size); break;
        case alphabeta: strncpy(buf, "(negamax alphabeta)", size);
    }
    return buf;
}

/* Utilisée dans le main pour gérer les arguments du programme. */
e_algo str_to_e_algo(char* str){
    if(strcmp(str, "arbre")==0)
        return arbre;
    if(strcmp(str, "leger")==0)
        return leger;
    return alphabeta;   //si on met n'importe quoi on a celle-là
}

/* Fonction qui joue k parties entre deux IA, comme affrontements_successifs,
 * mais mesure et affiche le temps total utilisé pour chaque IA à la fin.
 * algo_1 et algo_2 indiquent quelle fonction les IA utilisent. */
void test_de_performances(int k, int profondeur, e_algo algo_1, e_algo algo_2){
    int nb_coups_alea = (int)(log((double)k)/log(6.0)) + 1, res, coup, nb_v_j1 = 0, nb_v_j2 = 0, tt_pts_j1 = 0, tt_pts_j2 = 0;
    //                    calcul de log6(k)
    e_algo algos[2] = {algo_1, algo_2};
    clock_t ticks[2] = {0,0}, avant, apres;
    //ticks enregistre le nombre total de ticks écoulés pendant l'exécution de la fonction de décision,
    //pour les deux joueurs.
    for (int i = 0; i < k; i++) { // k parties
        int plateau[T_PLAT] = {INIT_TAB};
        Situation s = {plateau, 0, 0, 0, 0};
        coups_aleatoires(&s, nb_coups_alea);
        //si on commence du même point à chaque fois les parties seront strictement identiques, donc on fais des coups aléatoires
        while (1) {
            avant = clock();
            switch(algos[s.joueur_tour]){
                case arbre: ;
                    Noeud *n = nouvel_arbre(s, s.joueur_tour, profondeur, 0);
                    eval_arbre(n, s.joueur_tour, &coup);
                    free_arbre(n);
                    break;
                case leger:
                    minmax_leger(s, profondeur, s.joueur_tour, &coup);
                    break;
                case alphabeta:
                    negamax_alphabeta(s, profondeur, s.joueur_tour, &coup, -INFINI, INFINI, evaluation);
            }
            apres = clock();
            ticks[s.joueur_tour] += apres - avant;
            // calcul du prochain coup avec la fonction d'évaluation choisie pour ce joueur
            calcul_coup(&s, coup);
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
    char buf[64], buf2[64]; 
    printf("Temps totaux :\njoueur 1 %3$-19s : %1$.3f s\njoueur 2 %4$-19s : %2$.3f s\n", EN_SEC((double)ticks[0]), EN_SEC((double)ticks[1]),
    e_algo_to_str(algos[0], buf, 64), e_algo_to_str(algos[1], buf2, 64));
    printf("Taux de victoires :\nj1: %-7.4f, j2: %-7.4f\nPoints moyens :\nj1: %-7.4f, j2: %-7.4f\n",
    (float)nb_v_j1 / k, (float)nb_v_j2 / k, (float)tt_pts_j1 / k,(float)tt_pts_j2 / k);
}