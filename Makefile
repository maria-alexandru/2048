# Programarea Calculatoarelor, seria CC
# Tema2 - 2048

build: 2048

2048: 2048.c utils.o movement.o draw.o theme.o storage.o
	gcc -Wall utils.o movement.o draw.o theme.o storage.o 2048.c -o 2048 -lcurses

utils: utils.c
	gcc -Wall -c utils.c

movement: movement.c
	gcc -Wall -c movement.c

draw: draw.c
	gcc -Wall -c draw.c

theme: theme.c
	gcc -Wall -c theme.c

storage: storage.c
	gcc -Wall -c storage.c

.PHONY:

clean:
	rm -f 2048 utils.o movement.o draw.o theme.o storage.o

run:
	./2048
