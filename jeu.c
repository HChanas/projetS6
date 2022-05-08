#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>

/* Un tableau de 12 cases représente le plateau :
 * | 0 | 1 | 2 | 3 | 4 | 5 |
 * | 11| 10| 9 | 8 | 7 | 6 |
 * Cela permet de faciliter la répartition des tas en sens horaire. 
 */

int scan_entree(Situation s){
    int* cp = coups_possibles(s);
    for(int i=1, j=s.joueur_tour==0?0:T_PLAT-1; i<T_PLAT+1; i++, j+=s.joueur_tour==0?1:-1)
        if(cp[j])
            printf(" %d ", i);
        else
            printf("   ");
    printf("\nTour du joueur %d : Choisis un trou\n", s.joueur_tour+1);
    int entree=0;
    scanf("%d", &entree);
    entree += s.joueur_tour==1?(2*(T_PLAT/2-entree)):-1; //mathématiques pour convertir l'entree en id de case tu tableau
    while(trou_valide(cp, entree)){
        printf("Trou invalide.\n");
        scanf("%d", &entree);
        entree += s.joueur_tour==1?(2*(T_PLAT/2-entree)):-1;
    }
    free(cp);
    return entree;
}

/* Lance une partie entre deux joueurs humains (local). */
void partie_pvp(){
    int plateau[T_PLAT] = {INIT_TAB};
    Situation s = {plateau, 0, 0, 0};
    while(1){
        affiche_jeu(s);
        int entree = scan_entree(s);
        calcul_coup(&s, entree);
        switch(verif_fin(s)){
            case 1: printf("Victoire joueur 1\n"); return;
            case 2: printf("Victoire joueur 2\n"); return;
            case 3: printf("Egalité\n"); return;
            default: break;
        }
    }
}

/* Vérifie si la partie est finie.
 * 0 -> pas finie ; 1 -> joueur 1 a gagné ; 2 -> joueur 2 a gagné ; 3 -> égalité. */
int verif_fin(Situation s){
    if((s.pts_j1>=25)||(s.pts_j2>=25))
        return (s.pts_j2>s.pts_j1)+1; //celui qui a le plus de points
    if(s.pts_j1+s.pts_j2==48)
        return 3; //24 à 24
    return 0;
}

/* Affiche le plateau et le score des joueurs. */
void affiche_jeu(Situation s){
    printf("points: Joueur 1 : %d, Joueur 2 : %d\n", s.pts_j1, s.pts_j2);
    affiche_plateau(s.plateau);
}

/* Affiche les trous du plateau de jeu. */
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

/* Vérifie en fonction du tableau de coups possibles si l'entrée en est. */
//renvoie 1 si l'entree n'est pas un coup que peut jouer le joueur (en fonction de cp)
int trou_valide(int* cp, int entree){
    if((entree<0)||(entree>=T_PLAT))
        return 1;
    return !cp[entree];
}

/* Calcule les coups possibles pour le tour d'un joueur et renvoie
 * un tableau de la taille du plateau avec les trous possibles à 1. */
int* coups_possibles(Situation s){
    int* cp = malloc(sizeof(int)*T_PLAT); //tableau de coups possibles
    for(int i=0; i<T_PLAT; i++)
        cp[i]=0;
    int max = s.joueur_tour==0?T_PLAT/2:T_PLAT;
    for(int i = s.joueur_tour==0?0:T_PLAT/2; i<max; i++) //vérifie les trous du bon côté où il y a des billes
        if(s.plateau[i]!=0)
            cp[i] = 1;
    if(est_affame(s.plateau, 1-s.joueur_tour))
        coups_nourrissants(s, cp);
    return cp;
}

/* Donne simplement le nombre de coups possibles dans le tabeau (cases non nulles). */
int nb_cp(int* cp, int taille){
    int c = 0;
    for(int i=0; i<taille; i++)
        if(cp[i])
            c++;
    return c;
}

/* Réparti les billes du tas choisi dans les tas suivants selon le
 * principe de l'awalé. */
void repartition(Situation s, int* trou){
    if((*trou<0)||(*trou>11)) return;
    int depart = *trou;
    int nb_pierres = s.plateau[*trou];
    s.plateau[*trou] = 0;
    while(nb_pierres>0){
        (*trou)++;
        if(*trou==T_PLAT)
            *trou=0;
        if(*trou != depart){ //on ne met pas de graine dans le trou que l'on a joué
            s.plateau[*trou]++;
            nb_pierres--;
        }
    }
}

/* Vérifie si le coup 'affame' l'adversaire, ce qui signifie qu'on capture
 * toutes les graines du camp adverse. */
//le coup permet-il de manger toutes les graines adverses ? (si oui aucune capture n'est effectuée)
int affame(Situation s, int trou){
    int min = trou>5?6:0, i;
    for(i=min+(T_PLAT/2)-1; i>trou; i--)
        if(s.plateau[i]!=0)
            return 0; //une case avec des graines qui ne sera pas capturé

    for(; i>=min; i--)
        if((s.plateau[i]!=2)&&(s.plateau[i]!=3))
            return 0; //si il y a des cases dont les graines ne seront pas capturées
    return 1;
}

/* Vérifie les tas capturés selon les règles et calcule les points gagnés. */
void captures(Situation* s, int* trou){
    //tant que le trou a 2 ou 3 billes et est dans le camp adverse on prend
    if(!affame(*s, *trou)){//règle du jeu qui dit qu'on ne capture pas si
    //le coup permet de capturer toutes les graines du camp adverse (affamer)    
        while(((s->plateau[*trou]==2)||(s->plateau[*trou]==3))&&(s->joueur_tour!=camp(*trou))){
            if(s->joueur_tour) s->pts_j2 += s->plateau[*trou];
            else s->pts_j1 += s->plateau[*trou];
            s->plateau[*trou] = 0;
            (*trou)--;
            if(*trou<0)
                *trou = T_PLAT-1;
        }
    }
    s->nb_coups++;
    if(verif_famine(s))
        grande_capture_finale(s, s->joueur_tour==0, s->joueur_tour==1);
    if(s->nb_coups>NB_MAX_COUPS) //si la partie tourne en boucle (car le nb de coup est trop grand)
        grande_capture_finale(s, 1, 1);
    if(verif_partie_infinie(*s)) //détecte un cas particulier de partie infinie avant que le nb de coups soit trop grand...
        grande_capture_finale(s, 1, 1);
}

/* Utilise les fonction du jeu pour calculer le plateau et les points des joueurs si le coup indiqué est joué.
 * Cette fonction ressemble donc à tour_de_jeu(), mais sans demander le coup dans le terminal. 
 * Il faut vérifier avant d'appeler cette fonction quels sont les coups possibles. */
void calcul_coup(Situation *s, int coup) {
    // Répartition du tas en sens horaire
    repartition(*s, &coup);
    // Tas de billes mangés
    captures(s, &coup);
    // Au tour du joueur suivant
    s->joueur_tour = 1 - s->joueur_tour;
}

/* Dans quel camp est le trou (0 ou 1) -1 => trou invalide. */
int camp(int trou){
    if((trou>=0)&&(trou<T_PLAT/2))
        return 0;
    if((trou>=T_PLAT/2)&&(trou<T_PLAT))
        return 1;
    return -1;
}

/* Vérifie si il y a au moins une bille dans le camp du joueur donné (renvoie 0, 1 sinon). */
int est_affame(int* plateau, int joueur){
    int max = joueur==0?T_PLAT/2:T_PLAT;
    for(int i=0+(T_PLAT*joueur/2); i<max; i++)
        if(plateau[i]!=0)
            return 0;
    return 1;
}
/* Une règle stipule que si le joueur adverse est affamé (plus de graine dans son camp),
 * alors le joueur doit obligatoirement le nourrir (jouer un coup qui remet des graines).
 * Si le joueur ne peut nourrir son adversaire, alors il capture toutes les graines restantes et gagne.
 * 
 * Retire des coups possibles tous les coups qui ne nourrissent pas l'adversaire. */
void coups_nourrissants(Situation s, int* coups_possibles){
    int max = s.joueur_tour==0?T_PLAT/2:T_PLAT;
    for(int i = s.joueur_tour==0?0:T_PLAT/2; i<max; i++)
        if(coups_possibles[i]!=0){ //coup possible
            if(s.plateau[i]<max-i) //si il n'y a pas assez de graines pour nourrir l'adversaire
                coups_possibles[i] = 0;
        }
}

/* Tentative de détection des situations où le jeu dure indéfiniement. */
int verif_partie_infinie(Situation s){
    if((s.pts_j1+s.pts_j2)<46) return 0; //au moins trois graines
    int m=T_PLAT/2;
    for(int i=0; i<m; i++)
        if(s.plateau[i]&&s.plateau[i+m])
            return 1;
        //deux billes qui sont "à l'opposé" sur le plateau et qui "tournent" à l'infini
    return 0;
}

/* Vérifie si le joueur est affamé ET que son adversaire ne peut pas le nourrir.
 * Dans ce cas l'adversaire capture toutes les billes restantes (fin par famine). */
int verif_famine(Situation* s){
    if(est_affame(s->plateau, s->joueur_tour)){
        int max = s->joueur_tour?T_PLAT/2:T_PLAT;
        for(int i = s->joueur_tour?0:T_PLAT/2; i<max; i++)
            if(s->plateau[i]>=max-i) //si il y a assez de graines pour nourrir l'adversaire
                return 0;
        return 1;
    }
    return 0;
}

/* Toutes les graines qui restent sur le plateau sont capturées.
 * j1_capture et j2_capture indiquent si le joueur récupère les points ou non.
 * Les deux joueurs peuvent capturer les graines en même temps. */
void grande_capture_finale(Situation* s, int j1_capture, int j2_capture){
    for(int i=0; i<T_PLAT; i++)
            if(s->plateau[i]){
                if(j1_capture)
                    s->pts_j1 += s->plateau[i];
                if(j2_capture)
                    s->pts_j2 += s->plateau[i];
                s->plateau[i] = 0;
            }
}