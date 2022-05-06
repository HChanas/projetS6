#include "minmax.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int *cp = coups_possibles(s);
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

/// ELAGAGE ALPHA-BETA

/* Minmax en appliquant la logique de l'élagage alpha-beta. La fonction d'évaluation à utiliser est donnée. */

void minmax_alphabeta(Situation s, int profondeur, int joueur_a_maximiser, int *coup, int *alpha, int *beta, int (*eval)(Situation s, int joueur_a_maximiser)){
    printf("Pas implemente !\n");
    exit(1);
}

/// COMPARER LES FONCTIONS D'EVALUATION


/* Fonction qui joue n coups de façon aléatoire sur une partie. */
void coups_aleatoires(Situation *s, int n){
    for(int i=0; i<n; i++){
        int *cp = coups_possibles(*s);
        int ncp = nb_cp(cp, T_PLAT); //on compte les cases à 1
        int rd = rand()%ncp;  //un nombre entre 0 et le nombre de cases à 1
        int compteur=0, j;
        for(j=0; j<T_PLAT; j++){ //on fait correspondre le nombre tiré à la bonne case du tableau
            if(cp[j])
                compteur++;
            if(compteur>rd) break;
        }
        repartition(*s, &j);
        captures(s, &j);
        s->joueur_tour = 1 - s->joueur_tour;
    }
}

/* Fonction qui lance k parties entre deux IA, utilisant les fonctions d'évalutations ainsi que les profondeurs données.
 * Les k/6 premiers coups sont joués aléatoirement, et la fonction minmax_alphabeta est utilisée.
 * Le troisième argument prend un tableau de pointeurs de fonctions de taille 2 (donc les deux fonctions d'évalutations)*/

/* VERSION 'ALÉATOIRE' EN ATTENDANT QUE MINMAX ALPHA-BETA SOIR IMPLEMENTÉE */
Donnees affrontements_successifs(int k, int profondeurs[2], int (**eval)(Situation s, int joueur_a_maximiser)){
    int nb_coups_alea = k/6, res, /*alpha, beta, coup,*/ nb_v_j1=0, nb_v_j2=0, tt_pts_j1=0, tt_pts_j2=0;
    for(int i=0; i<k; i++){     //k parties
        int plateau[T_PLAT] = {INIT_TAB};
        Situation s = {plateau, 0, 0, 0};
        coups_aleatoires(&s, nb_coups_alea);
        while(1){
            /*minmax_alphabeta(s, profondeurs[s.joueur_tour], 1, &coup, &alpha, &beta, eval[s.joueur_tour]);*/
            //calcul du prochain coup avec la fonction d'évaluation choisie pour ce joueur
            coups_aleatoires(&s, 1);
            /*repartition(s, &coup);
            captures(&s, &coup);
            s.joueur_tour = 1 - s.joueur_tour;*/
            res = verif_fin(s);
            if(res==0) continue;
            tt_pts_j1+=s.pts_j1; tt_pts_j2+=s.pts_j2;
            if(res==1) nb_v_j1++;
            else if(res==2) nb_v_j2++;
            break;
        }
    }
    return (Donnees) {(float) nb_v_j1 / k, (float) nb_v_j2 / k, (float) tt_pts_j1 / k, (float) tt_pts_j2 / k};
}


void eval_arbre(Noeud* racine, int joueur_a_maximiser, int* coup) {
    // cas ou le neud correspond au joeur a maximiser : 
    int buff; // valeur intermediare qui sert a stocker la valeur minimum ou maximum a chaque fois  
    int i; // compteur pour parcourir les feuilles 
    if (racine->numero_joeur == joueur_a_maximiser) {
        // parcourrir les fils et prendre le minimum 
        buff = 999999999; //init le buff a une grande valeur   pour prendre même la premier valeur minimum  
        while (i < racine->feuille) {
            if (racine->fils[i] < buff ) {
                buff = racine->fils[i]; 
            }
            i++; 
        }
        coup = buff; // si j'ai bien compris comment a quio sert la variable coup  
    }
    // cas ou le neud ne correspond pas au joeur a maximiser : 
    else {
        // on prend le max des fils 
        buff = -1; // contraire a l'autre fonctionnement du min 
        // même chose pour la boucle 
        while (i < racine->feuille) {
            if (racine->fils[i] > buff ) {
                buff = racine->fils[i]; 
            }
            i++; 
        }
        coup = buff // A VERIFIER 
    }
    
}

