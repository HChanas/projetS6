SRC=jeu.c minmax.c main.c

run: awale
	./$^

awale: ${SRC} jeu.h
	gcc -o $@ -Wall ${SRC}