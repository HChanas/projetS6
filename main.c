#include "jeu.h"
#include "minmax.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Gère une partie d'awalé entre un joueur dans le terminal et une IA. 
 * L'IA est le joueur 2.*/
void jeu_solo(int profondeur, int joueur_humain){
    int plateau[T_PLAT] = {INIT_TAB};
    Situation s ={plateau, 0, 0, 0, 0};
    int entree, fn;

    while((fn=verif_fin(s))==0){ //tant que la partie n'est pas finie
        if(s.joueur_tour==joueur_humain){ //j1
            entree = scan_entree(s); //le coup joué est donné par le joueur dans le terminal
        }
        else{ //j2
            affiche_jeu(s);
            printf("Calcul des coups...\n");
            negamax_alphabeta(s,profondeur,s.joueur_tour,&entree,-INFINI,INFINI,eval_nb_graines_zeros);
            //le coup joué est donné par l'algo
            printf("Trou choisi : %d\n", entree+(2*(T_PLAT/2-entree)));
        }
        calcul_coup(&s, entree);
    }
    affiche_jeu(s);
    switch(verif_fin(s)){
        case 1: printf("Victoire joueur 1\n"); return;
        case 2: printf("Victoire joueur 2\n"); return;
        case 3: printf("Egalité\n"); return;
        default: break;
    }
}

/* Fonction qui renvoie un pointeur de fonction selon la chaine de cartactère donnée. */
int(*str_to_eval(char* str))(Situation,int){
    if(strcmp(str,"defaut")==0)
        return evaluation;
    if(strcmp(str,"nb_graines")==0)
        return eval_nb_graines;
    if(strcmp(str, "zeros")==0)
        return eval_nb_graines_zeros;
    return eval_nb_graines_zeros;
}

/* Gestion des arguments. */
int main(int argc, char** argv) {
    if(argc<2){
        printf("%s <mode> [args]\nmodes :\n"
                "pvc <profondeur> <joueur>: match contre une IA avec une profondeur d'arbre donnée\n"
                "perf <nb parties> <profondeur> <fct minmax 1> <fct minmax 2>: test de performance entre deux IA\n"
                "aff <nb parties> <prof j1> <prof j2> <fct d'eval 1> <fct d'eval 2>: k matchs entre deux IA\n",argv[0]);
        return 1;
    }
    srand(time(NULL));
    if(strcmp(argv[1],"pvc")==0){
        if(argc!=4){
            printf("%s pvc <profondeur> <joueur>:\n"
                    "profondeur: profondeur d'arbre utilisée par l'IA\n"
                    "joueur (0/1): 0 = joueur 1 (joue en premier), joueur 2 sinon\n", argv[0]);
            return 1;
        }
        jeu_solo(atoi(argv[2]), atoi(argv[3]));
    }
    else if(strcmp(argv[1],"perf")==0){
        if(argc!=6){
            printf("%s perf <nb parties> <profondeur> <fct minmax 1> <fct minmax 2>:\n"
                    "nb parties: nombre de parties jouées entre les IA pour le test\n"
                    "profondeur: profondeur d'arbre utilisée par les IA\n"
                    "fct minmax: fonction utilisée par l'IA (arbre, leger, alphabeta)\n", argv[0]);
            return 1;
        }
        test_de_performances(atoi(argv[2]), atoi(argv[3]), str_to_e_algo(argv[4]), str_to_e_algo(argv[5]));
    }
    else if(strcmp(argv[1],"aff")==0){
        if(argc<7){
            printf("%s aff <nb parties> <prof j1> <prof j2> <fct d'eval 1> <fct d'eval 2>:\n"
                    "nb parties: nombre d'affrontements entre les deux IA\n"
                    "prof: profondeur d'arbre utilisée par l'IA\n"
                    "fct d'eval: fonction d'évaluation utilisée par l'IA (defaut, nb_graines, zeros)\n", argv[0]);
            return 1;
        }
        Donnees d = affrontements_successifs(atoi(argv[2]), (int[2]){atoi(argv[3]),atoi(argv[4])}, (int(*[2])(Situation,int)){str_to_eval(argv[5]), str_to_eval(argv[6])});
        printf("Taux de victoires :\nj1 (%5$s): %1$-7.4f\nj2 (%6$s): %2$-7.4f\nScore moyen :\nj1 (%5$s): %3$-7.4f\nj2 (%6$s): %4$-7.4f\n",
        d.taux_v1, d.taux_v2, d.moy_pts_j1, d.moy_pts_j2, argv[5], argv[6]);
    }
    else{
        printf("%s <mode> [args]\nmodes :\n"
                "pvc <profondeur>: match contre une IA avec une profondeur d'arbre donnée\n"
                "perf <nb parties> <profondeur> <fct minmax 1> <fct minmax 2>: test de performance entre deux IA\n"
                "aff <nb parties> <prof j1> <prof j2> <fct d'eval 1> <fct d'eval 2>: k matchs entre deux IA\n",argv[0]);
        return 1;
    }
    return 0;
}