CFLAGS=-g -std=gnu99
LDFLAGS = -lpcre

SOURCES=$(wildcard src/*.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))

LEMON_SOURCES=$(wildcard src/*.y)
LEMON_OBJECTS=$(patsubst %.y, %.o, $(LEMON_SOURCES)) \
	$(patsubst %.y, %.c, $(LEMON_SOURCES)) \
	$(patsubst %.y, %.out, $(LEMON_SOURCES)) \
	$(patsubst %.y, %.h, $(LEMON_SOURCES))

TEST_SOURCES=$(wildcard tests/*_tests.check)
TEST_OBJECTS=$(patsubst %.check, %.o, $(TEST_SOURCES)) \
	$(patsubst %.check, %.c, $(TEST_SOURCES))
TESTS=$(patsubst %.check, bin/%, $(TEST_SOURCES))

all: bin/calculator

bin/calculator: bin parser $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) src/gram.o

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: test clean

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

bin/tests/%_tests: tests/%_tests.c parser bin/tests
	$(CC) $(CFLAGS) `pkg-config --cflags --libs check` -o $@ $< src/gram.c src/hashmap.c
	$@

clean:
	rm -rf bin $(OBJECTS) $(LEMON_OBJECTS) $(TEST_OBJECTS)
