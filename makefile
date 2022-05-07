SRC=jeu.c minmax.c main.c

run: awale
	./$^ 50 6 5

awale: ${SRC} jeu.h
	gcc -Wall -Werror -fsanitize=address -o $@ ${SRC} -lm