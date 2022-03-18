run: cmpl
	./awale

cmpl: jeu.c jeu.h
	gcc -o awale -Wall jeu.c