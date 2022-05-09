SRC=jeu.c minmax.c main.c

run: awale
	./$^ pvc 10

awale: ${SRC} jeu.h
	gcc -Wall -Werror -fsanitize=address -o $@ ${SRC} -lm

clean:
	rm -f awale