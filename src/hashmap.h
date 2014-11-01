#ifndef HASHMAP_H
#define HASHMAP_H

#ifndef HASHMAP_INITIAL_CAPACITY
#define HASHMAP_INITIAL_CAPACITY 32
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hashmap_entry {
	char *key;
	void *value;
};

struct hashmap {
	int capacity;
	int size;
	struct hashmap_entry *values;
};

void hashmap_free(struct hashmap *table);
void hashmap_put(struct hashmap **table, const char *key, void *value);
void *hashmap_get(struct hashmap *table, const char *key);

#endif				/* end of include guard: HASHTABLE_H */
