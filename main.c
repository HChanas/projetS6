#include "jeu.h"
#include "minmax.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  int plateau[T_PLAT] = {INIT_TAB};
  Situation initiale = {plateau, 0, 0, 0};
  Noeud *racine = nouvel_arbre(initiale, 0, 4,0);
  print_tree(racine, 3);
  free_arbre(racine);
  return 0;
}
