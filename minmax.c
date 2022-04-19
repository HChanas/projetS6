#include "minimax.h"

/* Initialise une situation en fonction des arguments donnés. */
Situation nouvelle_situation(int* plateau, int joueur, int pts_j1, int pts_j2){
    Situation s = {plateau, pts_j1, pts_j2, joueur};
    return s;
}

/* Fonction d'évaluation. */
int evaluation(Situation s, int joueur){
    return joueur==0 ? s.pts_j1-s.pts_j2 : s.pts_j2-s.pts_j1;
}