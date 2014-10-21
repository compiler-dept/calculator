CFLAGS  = -Wall -g -std=gnu99
LDFLAGS = -lpcre

all: parser calculator.o gram.o lexer.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o calculator calculator.o gram.o lexer.o

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

lemon: lemon.c
	$(CC) -o lemon lemon.c

.PHONY: parser test parser_test clean

parser: gram.y lemon
	./lemon gram.y

test/%.c: test/%.check
	checkmk $< > $@

test/bin/%_test.o: test/%_test.c
	mkdir -p test/bin
	$(CC) -c $(CFLAGS) $< -o $@ `pkg-config --cflags check`

test/bin/%_test: test/bin/%_test.o gram.o hashmap.o
	$(CC) -o $@ $^ `pkg-config --cflags --libs check`

test: parser test/bin/parser_test test/bin/hashmap_test
	./test/bin/parser_test
	./test/bin/hashmap_test


clean:
	rm -rf *.o *.out calculator lemon gram.c gram.h test/*.c test/bin
