run: cmpl
	./awale

cmpl: jeu.c main.c jeu.h
	gcc -o awale -Wall main.c jeu.c