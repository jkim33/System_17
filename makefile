all: control.c semaphone.c
	gcc -o control control.c
	gcc -o semaphone semaphone.c

