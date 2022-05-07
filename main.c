#include "jeu.h"
#include "minmax.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void jeu_solo(){
    int plateau[T_PLAT] = {INIT_TAB};
    Situation s ={plateau, 0, 0, 0, 0};
    int entree, fn;

    while((fn=verif_fin(s))==0){
        affiche_jeu(s);
        if(s.joueur_tour==0){
            int* cp = coups_possibles(s);
            for(int i=1, j=0; i<T_PLAT+1; i++, j++)
                if(cp[j])
                    printf(" %d ", i);
                else
                    printf("   ");
            printf("\nChoisis un trou\n");
            char buf[16];
            scanf("%s", buf); entree = atoi(buf);
            entree--;
            while(trou_valide(cp, entree)){
                printf("Trou invalide.\n");
                scanf("%s", buf); entree = atoi(buf);
                entree--;
            }
            free(cp);
        }
        else{
            printf("Calcul des coups...\n");
            negamax_alphabeta(s, 9, 1, &entree, -INFINI, INFINI, evaluation);
            printf("Trou choisi : %d\n", entree+(2*(T_PLAT/2-entree)));
        }
        //Répartition du tas en sens horaire
        repartition(s, &entree);
        //Tas de billes mangés
        captures(&s, &entree);
        s.joueur_tour = 1 - s.joueur_tour;
    }
    switch(verif_fin(s)){
        case 1: printf("Victoire joueur 1\n"); return;
        case 2: printf("Victoire joueur 2\n"); return;
        case 3: printf("Egalite\n"); return;
        default: break;
    }
}

int nb_appels = 0;
int nb_cps = 0;
int nb_coupures = 0;
int nb_feuilles = 0;
// ./awale nb_parties prof1 prof2
int main(int argc, char** argv) {
    printf("%s <nombre de parties> <profondeur j1> <profondeur j2>\n", argv[0]);
    if(argc!=4) return 1;
    srand(time(NULL)); 
    Donnees d = affrontements_successifs(atoi(argv[1]), (int[2]){atoi(argv[2]),atoi(argv[3])}, (int(*[2])(Situation,int)){evaluation, evaluation});
    printf("Taux de victoires :\nj1: %-7.4f, j2: %-7.4f\nPoints moyens :\nj1: %-7.4f, j2: %-7.4f\n", d.taux_v1, d.taux_v2, d.moy_pts_j1, d.moy_pts_j2);

    return 0;
}