#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct Noeud Noeud;
struct Noeud {
  int nombre_de_points; // stocke le resultat pour les calculs
  int numero_move;
  int numero_joeur;
  Noeud *suivant; // adresse de l'element suivant dans la liste
  Noeud *un;
  Noeud *deux;
  Noeud *trois;
  Noeud *quatre;
  Noeud *cinq;
  Noeud *six;
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
void insertion(Liste *liste, int numero_move, int numero_joeur) {
  Noeud *nouveau = malloc(sizeof(*nouveau));
  if (liste == NULL || nouveau == NULL) {
    exit(EXIT_FAILURE);
  }
  nouveau->numero_move = numero_move;
  nouveau->numero_joeur = numero_joeur;
  nouveau->un = NULL;
  nouveau->deux = NULL;
  nouveau->trois = NULL;
  nouveau->quatre = NULL;
  nouveau->cinq = NULL;
  nouveau->six = NULL;
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

// Liste *nouvel_arbre(int *plateau, int joueur, int profondeur) {}

int main(int argc, char *argv[]) {
  int resultat;
  Liste *liste = initialisation();

  /* Génère un arbre de possibilités à partir d'un plateau de jeu et du joueur
   * qui va jouer. La profondeur de l'arbre est précisée. */
}