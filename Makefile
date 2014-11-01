CFLAGS=-g -std=gnu99
LDFLAGS = -lpcre

SOURCES=$(wildcard src/*.c)
OBJECTS=$(patsubst src/%.c, bin/%.o, $(SOURCES))

LIBCOLLECT_OBJ=$(patsubst %, bin/%, hashmap.o stack.o)

LEMON_SOURCES=$(wildcard src/*.y)
LEMON_OBJECTS=$(patsubst %.y, %.o, $(LEMON_SOURCES)) \
	$(patsubst %.y, %.c, $(LEMON_SOURCES)) \
	$(patsubst %.y, %.out, $(LEMON_SOURCES)) \
	$(patsubst %.y, %.h, $(LEMON_SOURCES))

TEST_SOURCES=$(wildcard tests/*_tests.check)
TEST_OBJECTS=$(patsubst %.check, %.o, $(TEST_SOURCES)) \
	$(patsubst %.check, %.c, $(TEST_SOURCES))
TESTS=$(patsubst %.check, bin/%, $(TEST_SOURCES))

.PRECIOUS: $(TESTS)

all: bin/calculator

bin/calculator: bin parser $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) src/gram.o

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: test clean indent

test: $(TESTS)

bin:
	mkdir -p bin

bin/tests:
	mkdir -p bin/tests

bin/lemon: src/lemon/lemon.c bin
	$(CC) $(CFLAGS) -o $@ $<

parser: src/gram.y bin/lemon
	bin/lemon T=src/lemon/lempar.c $<

tests/%_tests.c: tests/%_tests.check
	checkmk $< > $@

bin/tests/%_tests: tests/%_tests.c parser libcollect bin/tests
	$(CC) $(CFLAGS) `pkg-config --cflags --libs check` -o $@ $< src/gram.c src/ast.c src/ast_eval.c -Llib -lcollect
	$@

lib:
	mkdir -p lib

libcollect: $(LIBCOLLECT_OBJ) lib
	ar -rcs lib/libcollect.a $(LIBCOLLECT_OBJ)
	

clean:
	rm -rf bin lib $(OBJECTS) $(LEMON_OBJECTS) $(TEST_OBJECTS)

indent:
	find . \( \( -iname "*.c" -o -iname "*.h" \) -a -path ./lemon -prune \) -exec indent -linux {} \;
