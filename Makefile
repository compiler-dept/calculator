CFLAGS = -Wall -g -std=c99

all: main.o list.o
	$(CC) $(CFLAGS) -o main main.o list.o


main.o: main.c parser

.PHONY: parser
parser: list.y lemon
	./lemon list.y

lemon: lemon.c
	$(CC) -o lemon lemon.c
