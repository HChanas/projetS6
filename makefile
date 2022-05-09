SRC=jeu.c minmax.c main.c

run: awale
	./$^ pvc 10

awale: ${SRC} jeu.h
	gcc -Wall -Werror -o $@ ${SRC} -lm

clean:
	rm -f awale