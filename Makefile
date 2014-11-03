CFLAGS=-g -std=gnu99
LDFLAGS=-Llib -lcollect
YACC=lemon
LEX=flex

LIBCOLLECT_OBJ=$(patsubst %.c, %.o, src/hashmap.c src/stack.c)

NOWANTS=src/lexer.c src/gram.c src/hashmap.c src/stack.c
SOURCES=$(wildcard src/*.c) $(wildcard src/*.l) $(wildcard src/*.y)
OBJECTS=$(patsubst %.c, %.o, $(patsubst %.l, %.o, $(patsubst %.y, %.o, $(filter-out $(NOWANTS), $(SOURCES)))))

TEST_SOURCES=$(wildcard tests/*_tests.check)
TEST_OBJECTS=$(patsubst %.check, %.o, $(TEST_SOURCES)) \
	$(patsubst %.check, %.c, $(TEST_SOURCES))
TESTS=$(patsubst %.check, bin/%, $(TEST_SOURCES))

.PHONY: all parser test clean indent objects

all: bin/calculator

bin/calculator: $(OBJECTS) bin libcollect
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

src/calculator.o: src/calculator.c src/lexer.c

parser: src/gram.y
	$(YACC) $<

src/lexer.c: src/lexer.l parser
	$(LEX) --header-file=src/lexer.h -o $@ $<

bin:
	mkdir -p bin

bin/tests:
	mkdir -p bin/tests

lib:
	mkdir -p lib

tests/%_tests.c: tests/%_tests.check
	checkmk $< > $@

bin/tests/%_tests: tests/%_tests.c src/lexer.c libcollect bin/tests
	$(CC) $(CFLAGS) $(LDFLAGS) `pkg-config --cflags --libs check` -o $@ $< src/gram.c src/ast.c src/ast_eval.c src/lexer.c
	$@

libcollect: $(LIBCOLLECT_OBJ) lib
	ar -rcs lib/libcollect.a $(LIBCOLLECT_OBJ)

test: $(TESTS)

objects:
	@- echo $(OBJECTS)

clean:
	rm -rf bin lib $(OBJECTS) src/gram.h src/gram.c src/gram.out src/lexer.c src/lexer.h

indent:
	find . -not -path "*/lemon/*" \( \( -iname "*.c" -o -iname "*.h" \) \) -exec indent -linux {} \;
