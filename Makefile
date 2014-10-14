CFLAGS  = -Wall -g -std=gnu99
LDFLAGS = -lpcre

all: parser calculator.o gram.o lexer.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o calculator calculator.o gram.o lexer.o

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $< -o $@

lemon: lemon.c
	$(CC) -o lemon lemon.c

.PHONY: parser test parser_test clean

parser: gram.y lemon
	./lemon gram.y

test/%.c: test/%.check
	checkmk $< > $@

parser_test: test/parser_test.c parser
	mkdir -p test/bin
	$(CC) -o test/bin/$@ $< gram.c `pkg-config --cflags --libs check`

test: parser_test
	./test/bin/parser_test

clean:
	rm -rf *.o *.out calculator lemon gram.c gram.h test/*.c test/bin
