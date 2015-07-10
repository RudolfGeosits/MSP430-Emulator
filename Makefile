all: main

main: main.c
	gcc -pg main.c -o MSP430 -lreadline `pkg-config --cflags --libs gtk+-2.0`
