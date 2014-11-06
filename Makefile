CFLAGS=-g -Wall -std=c11 -Ilibcollect
LDFLAGS=-Llibcollect -lcollect
YACC=lemon/lemon
LEX=flex

NOWANTS=src/lexer.c src/gram.c src/hashmap.c src/stack.c
SOURCES=$(wildcard src/*.c) $(wildcard src/*.l) $(wildcard src/*.y)
OBJECTS=$(patsubst %.c, %.o, $(patsubst %.l, %.o, $(patsubst %.y, %.o, $(filter-out $(NOWANTS), $(SOURCES)))))

TEST_SOURCES=$(wildcard tests/*_tests.check)
TEST_OBJECTS=$(patsubst %.check, %.o, $(TEST_SOURCES)) \
	$(patsubst %.check, %.c, $(TEST_SOURCES))
TESTS=$(patsubst %.check, bin/%, $(TEST_SOURCES))

.PRECIOUS: $(TESTS)
.PHONY: all parser lemon test clean dist-clean indent objects libcollect getexternals

all: bin/calculator

bin/calculator: $(OBJECTS) bin libcollect
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

src/calculator.o: src/calculator.c src/lexer.c

parser: src/gram.y lemon
	$(YACC) $<

src/lexer.c: src/lexer.l parser
	$(LEX) --header-file=src/lexer.h -o $@ $<

bin:
	mkdir -p bin

bin/tests:
	mkdir -p bin/tests

tests/%_tests.c: tests/%_tests.check
	checkmk $< > $@

bin/tests/%_tests: tests/%_tests.c src/lexer.c libcollect bin/tests
	$(CC) $(CFLAGS) `pkg-config --cflags --libs check` -o $@ $< src/gram.c src/ast.c src/ast_eval.c src/lexer.c $(LDFLAGS)
	$@

libcollect:
	@- make -C libcollect

lemon:
	@- make -C lemon

test: $(TESTS)

objects:
	@- echo $(OBJECTS)

clean:
	rm -rf bin lib $(OBJECTS) src/gram.h src/gram.c src/gram.out src/lexer.c src/lexer.h

dist-clean:
	@- make clean
	@- make -C libcollect clean
	@- make -C lemon clean

indent:
	find . \( -iname "*.c" -o -iname "*.h" \) -exec astyle --style=linux {} \;

getexternals:
	git submodule init
	git submodule update
