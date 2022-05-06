SRC=jeu.c minmax.c main.c

run: awale
	./$^

awale: ${SRC} jeu.h
	gcc -Wall -Werror -fsanitize=address -o $@ ${SRC}