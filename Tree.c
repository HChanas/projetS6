#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define COUNT 10
#define N_DECIMAL_POINTS_PRECISION (10) // n = 3. Three decimal points.
#define SIZE 100

/* 3=op 3.1=+ 3.2=. 3.3= => 3.4 <=> 3.5 = NON
 * 2=constante 2.1=0 2.1 =1
 * 1= parenthese 1.1 = ( 1.2 = )
 */

typedef struct Noeud Noeud;
struct Noeud {
  char type[50];   // stocke le type operateur parenthese constante
  char valeur[50]; // stocke la valeur precise => ( ....
  int resultat;    // stocke le resultat pour les calculs
  Noeud *suivant;  // adresse de l'element suivant dans la liste
  Noeud *droit;
  Noeud *gauche;
};

typedef struct Liste Liste;
struct Liste {
  Noeud *head;   // stocke le debut de la liste
  Noeud *tail;   // stocke la fin de la liste;
  Noeud *racine; // stocke la racine de l'arbre
};

Liste *initialisation() {
  Liste *liste = malloc(sizeof(*liste));

  if (liste == NULL) {
    exit(EXIT_FAILURE);
  }
  liste->head = liste->tail = NULL;
  return liste;
}
// ajoute un element de la liste a la fin de la chaine
void insertion(Liste *liste, char *type, char *valeur) {
  Noeud *nouveau = malloc(sizeof(*nouveau));
  if (liste == NULL || nouveau == NULL) {
    exit(EXIT_FAILURE);
  }
  strcpy(nouveau->type, type);
  strcpy(nouveau->valeur, valeur);
  if (strcmp(nouveau->type, "constante") == 0) {
    nouveau->resultat = atoi(nouveau->valeur);
  } else {
    nouveau->resultat = 5;
  }
  nouveau->droit = NULL;
  nouveau->gauche = NULL;
  nouveau->suivant = NULL;

  if (nouveau == NULL) {
    printf("not initialized\n");
    free(nouveau);
  } else if (NULL == liste->head && NULL == liste->tail) {

    liste->head = liste->tail = nouveau;
  } else if (NULL == liste->head || NULL == liste->tail) {
    fprintf(stderr, "ERROR head ou tail vide\n");
    free(nouveau);
  } else {
    /* printf("List not empty, adding element to tail\n"); */
    liste->tail->suivant = nouveau;
    liste->tail = nouveau;
  }
}

void afficherListe(Liste *liste) {
  if (liste == NULL) {
    exit(EXIT_FAILURE);
  }
  Noeud *actuel = NULL;

  for (actuel = liste->head; actuel; actuel = actuel->suivant) {
    printf("%s", actuel->valeur);
  }
  printf("\n");
}
// enleve un element de la liste a la fin
void list_remove_element(Liste *liste) {
  Noeud *h = NULL;
  Noeud *p = NULL;

  if (NULL == liste) {
    printf("List is empty\n");
  } else if (NULL == liste->head && NULL == liste->tail) {
    printf("Well, List is empty\n");
  } else if (NULL == liste->head || NULL == liste->tail) {
    printf("There is something seriously wrong with your list\n");
    printf("One of the head/tail is empty while other is not \n");
  }

  h = liste->head;
  p = h->suivant;
  free(h);
  liste->head = p;
}
// supprime jusqu'a ce que la liste soit vide
void suppression_chaine(Liste *liste) {
  while (liste->head != NULL) {
    list_remove_element(liste);
  }
  free(liste);
}

int main(int argc, char *argv[]) {
  int resultat;
  Liste *liste = initialisation();
}