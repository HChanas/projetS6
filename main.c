#include "jeu.h"
#include "minmax.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  int plateau[T_PLAT] = {INIT_TAB};
  Situation initiale = {plateau, 0, 0, 0};
  Noeud racine = *nouvelle_arbre(3, 1, initiale, 0, 1);
  print_tree(&racine, 3);
  free_arbre(&racine);
  return 0;
}
