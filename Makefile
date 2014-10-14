CFLAGS  = -Wall -g -std=gnu99
LDFLAGS = -lpcre

all: parser calculator.o gram.o lexer.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o calculator calculator.o gram.o lexer.o

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

.PHONY: parser clean

parser: gram.y lemon
	./lemon gram.y

lemon: lemon.c
	$(CC) -o lemon lemon.c

clean:
	rm -f *.o *.out calculator lemon gram.c gram.h
