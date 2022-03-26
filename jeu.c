#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>

/* Un tableau de 12 cases représente le plateau :
 * | 0 | 1 | 2 | 3 | 4 | 5 |
 * | 11| 10| 9 | 8 | 7 | 6 |
 * Cela permet de faciliter la répartition des tas en sens horaire. 
 */


int main(int argc, char** argv){
    int plateau[T_PLAT] = {INIT_TAB}, pts_j[2]={0,0}, j=0, un=1;
    while(un){
        affiche_jeu(plateau, pts_j[0], pts_j[1]);
        tour_de_jeu(plateau, j, &pts_j[j]);
        j = 1-j; // 0 <-> 1
        for(int i=0; i<2; i++){
            if(pts_j[i]>=25){
                printf("Victoire joueur %d\n", i+1);
                un=0;
            }
        }
    }
    return 0;
}

void affiche_jeu(int* plateau, int pts_j1, int pts_j2){
    printf("points: Joueur 1 : %d, Joueur 2 : %d\n", pts_j1, pts_j2);
    affiche_plateau(plateau);
}

//Affiche les trous du plateau avec le nombre de billes (celui du joueur quoi selon si tu es d'un côté ou de l'autre du plateau faut se représenter mais l'immersion est là on s'y croirait)
void affiche_plateau(int* plateau){
    int moitie = (T_PLAT/2)-1;
    printf("Joueur 1\n");
    for(int i=0; i<moitie; i++)
        printf("|%-2d", plateau[i]);
    printf("|%-2d|\n", plateau[moitie]);
    for(int i=T_PLAT-1; i>moitie+1; i--)
        printf("|%-2d", plateau[i]);
    printf("|%-2d|\n", plateau[moitie+1]);
    printf("Joueur 2\n");
}

//demande une entrée correspondant à une trou et réparti les pierres comme dans les règles du jeu regarde sur la page wikipédia tu crois que je vais détailler les règles ici ?
void tour_de_jeu(int* plateau, int joueur, int* pts_joueur){
    int* cp = coups_possibles(plateau, joueur);
    for(int i=1, j=joueur==0?0:T_PLAT-1; i<T_PLAT+1; i++, j+=joueur==0?1:-1)
        if(cp[j])
            printf(" %d ", i);
        else
            printf("   ");
    printf("\nTour du joueur %d : Choisis un trou maintenant ou conséquences\n", joueur+1);
    int entree; char *buf = malloc(16);
    scanf("%s", buf); entree = atoi(buf);
    entree += joueur==1?(2*(T_PLAT/2-entree)):-1; //mathématiques pour convertir l'entree en id de case tu tableau
    while(trou_valide(cp, entree)){
        printf("Trou invalide.\n");
        scanf("%s", buf); entree = atoi(buf);
        entree += joueur==1?(2*(T_PLAT/2-entree)):-1;
    }
    //numéro de trou demandé : 1 - 6
    //camp du joueur 1 (0) : trous 0 - 5
    //camp du joueur 2 (1) : trous 11 - 6 (besoin de convertir)

    //Répartition du tas en sens horaire
    repartition(plateau, &entree);
    //Tas de billes mangés
    captures(plateau, &entree, pts_joueur, joueur);
}

//renvoie 1 si l'entree n'est pas un coup que peut jouer le joueur (en fonction de cp)
int trou_valide(int* cp, int entree){
    if((entree<0)||(entree>=T_PLAT))
        return 1;
    return !cp[entree];
}

//calcule les coups possibles pour le tour d'un joueur
//se contente de regarder les trous vides pour le moment
int* coups_possibles(int* plateau, int joueur){
    int* cp = malloc(sizeof(int)*T_PLAT); //tableau de coups possibles
    for(int i=0; i<T_PLAT; i++)
        cp[i]=0;
    int max = joueur==0?T_PLAT/2:T_PLAT;
    for(int i = joueur==0?0:T_PLAT/2; i<max; i++) //vérifie les trous du bon côté où il y a des billes
        if(plateau[i]!=0)
            cp[i] = 1;
    if(est_affame(plateau, 1-joueur))
        coups_nourrissants(plateau, joueur, cp);
    return cp;
}

void repartition(int* plateau, int* trou){
    int depart = *trou;
    int nb_pierres = plateau[*trou];
    plateau[*trou] = 0;
    while(nb_pierres>0){
        (*trou)++;
        if(*trou==T_PLAT)
            *trou=0;
        if(*trou != depart){ //on ne met pas de graine dans le trou que l'on a joué
            plateau[*trou]++;
            nb_pierres--;
        }
    }
}

//le coup permet-il de manger toutes les graines adverses ? (si oui aucune capture n'est effectuée)
int affame(int* plateau, int trou){
    int min = trou>5?6:0, i;
    for(i=min+(T_PLAT/2)-1; i>trou; i--)
        if(plateau[i]!=0)
            return 0; //une case avec des graines qui ne sera pas capturé

    for(; i>=min; i--)
        if((plateau[i]!=2)&&(plateau[i]!=3))
            return 0; //si il y a des cases dont les graines ne seront pas capturées
    return 1;
}

void captures(int* plateau, int* trou, int* pts_joueur, int joueur){
    //tant que le trou a 2 ou 3 billes et est dans le camp adverse on prend
    if(affame(plateau, *trou))//règle du jeu qui dit qu'on ne capture pas si
    //le coup permet de capturer toutes les graines du camp adverse (affamer)
        return;
    while(((plateau[*trou]==2)||(plateau[*trou]==3))&&(joueur!=camp(*trou))){
        *pts_joueur += plateau[*trou];
        plateau[*trou] = 0;
        (*trou)--;
        if(*trou<0)
            *trou = T_PLAT-1;
    }
}

//dans quel camp est le trou (1 ou 2) 0 => erreur
int camp(int trou){
    if((trou>=0)&&(trou<T_PLAT/2))
        return 0;
    if((trou>=T_PLAT/2)&&(trou<T_PLAT))
        return 1;
    return -1;
}

int est_affame(int* plateau, int joueur){
    int max = joueur==0?T_PLAT/2:T_PLAT;
    for(int i=0+(T_PLAT*joueur/2); i<max; i++)
        if(plateau[i]!=0)
            return 0;
    return 1;
}
//utile si l'adversaire est affamé, pour ajuster les coups possibles
void coups_nourrissants(int* plateau, int joueur, int* coups_possibles){
    int max = joueur==0?T_PLAT/2:T_PLAT;
    for(int i = joueur==0?0:T_PLAT/2; i<max; i++)
        if(coups_possibles[i]!=0){ //coup possible
            if(plateau[i]<max-i) //si il n'y a pas assez de graines pour nourrir l'adversaire
                coups_possibles[i] = 0;
        }
}