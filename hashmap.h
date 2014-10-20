#ifndef HASHMAP_H

#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct hashmap_entry {
    char* key;
    int value;
};

struct hashmap {
	int capacity;
	int size;
	struct hashmap_entry *values;
};

struct hashmap *hashmap_alloc(int capacity);
void hashmap_free(struct hashmap *table);
struct hashmap *hashmap_put(struct hashmap *table, const char *key,
		       int value);
int hashmap_get(struct hashmap *table, const char *key);

#endif /* end of include guard: HASHTABLE_H */
