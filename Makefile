CFLAGS = -Wall -g -std=gnu99
LDFLAGS = -lpcre

all: parser main.o list.o lexer.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o main main.o list.o lexer.o


%.o: %.c %.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@


.PHONY: parser
parser: list.y lemon
	./lemon list.y


lemon: lemon.c
	$(CC) -o lemon lemon.c


.PHONY: clean
clean:
	rm -f *.o *.out  main lemon
