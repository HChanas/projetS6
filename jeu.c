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

//Affiche les cases du plateau avec le nombre de billes (celui du joueur quoi selon si tu es d'un côté ou de l'autre du plateau faut se représenter mais l'immersion est là on s'y croirait)
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

//demande une entrée correspondant à une case et réparti les pierres comme dans les règles du jeu regarde sur la page wikipédia tu crois que je vais détailler les règles ici ?
void tour_de_jeu(int* plateau, int joueur, int* pts_joueur){
    int* cp = coups_possibles(plateau, joueur);
    for(int i=1, j=joueur==0?0:T_PLAT-1; i<T_PLAT+1; i++, j+=joueur==0?1:-1)
        if(cp[j])
            printf(" %d ", i);
        else
            printf("   ");
    printf("\nTour du joueur %d : Choisis une case maintenant ou conséquences\n", joueur+1);
    int entree; char *buf = malloc(16);
    scanf("%s", buf); entree = atoi(buf);
    entree += joueur==1?(2*(T_PLAT/2-entree)):-1;
    while(!cp[entree]){
        printf("Case invalide.\n");
        scanf("%s", buf); entree = atoi(buf);
        entree += joueur==1?(2*(T_PLAT/2-entree)):-1;
    }
    //numéro de case demandé : 1 - 6
    //camp du joueur 1 (0) : cases 0 - 5
    //camp du joueur 2 (1) : cases 11 - 6 (besoin de convertir)

    int nb_pierres = plateau[entree];
    plateau[entree] = 0;
    //Répartition du tas en sens horaire
    while(nb_pierres>0){
        entree++;
        if(entree==T_PLAT)
            entree=0;
        plateau[entree]++;
        nb_pierres--;
    }
    //Tas de billes mangés
    while((plateau[entree]==2)||(plateau[entree]==3)){
        *pts_joueur += plateau[entree];
        plateau[entree] = 0;
        entree--;
        if(entree<0)
            entree = T_PLAT-1;
    }
}

//calcule les coups possibles pour le tour d'un joueur
//se contente de regarder les cases vides pour le moment
int* coups_possibles(int* plateau, int joueur){
    int* cp = malloc(sizeof(int)*T_PLAT);
    for(int i=0; i<T_PLAT; i++)
        cp[i]=0;
    int max = joueur==0?T_PLAT/2:T_PLAT;
    for(int i = joueur==0?0:T_PLAT/2; i<max; i++)
        if(plateau[i]!=0)
            cp[i] = 1;
    return cp;
}