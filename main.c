#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jeu.h"
#include "minmax.h"

int main(){
    //partie_pvp();
    srand(time(NULL));
    printf("Statistiques de 100 parties jouees aleatoirement :\n");
    Donnees d = affrontements_successifs(100, (int[2]) {0,0}, (int(*[])(Situation s, int joueur_a_maximiser)){evaluation, evaluation});
    //                                                                           vas-y chiale
    printf("%f, %f ; %f, %f\n", d.taux_v1, d.taux_v2, d.moy_pts_j1, d.moy_pts_j2);
    return 0;
}
