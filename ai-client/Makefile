default: console

console: main.c ../shared/board.c ../shared/protocol.c ../shared/protocol_platform_wiringpi.c
	gcc -Wall -o main.o main.c -lwiringPi

clean:
	-rm -f main.o
	-rm -f main
