#include "hashmap.h"

/**
 * @brief Dan Bernsteins DJB2 Hash Function
 * @detail http://www.cse.yorku.ca/~oz/hash.html
 * String must be terminated with a null-byte.
 */
unsigned long hash(unsigned char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c;	/* hash * 33 + c */

	return hash;
}

/**
 * @brief Allocate new hash table.
 * @detail Uses malloc. Free with hashmap_free.
 */
struct hashmap *hashmap_alloc(int capacity)
{
	struct hashmap *new_table = malloc(sizeof(struct hashmap));
	new_table->capacity = capacity;
	new_table->size = 0;
	new_table->values = malloc(capacity * sizeof(struct hashmap_entry));
	for (int i = 0; i < new_table->capacity; i++) {
		new_table->values[i].key = NULL;
	}
	return new_table;
}

/**
 * @brief Free hash table.
 * @detail Uses free.
 */
void hashmap_free(struct hashmap *table)
{
	for (int i = 0; i < table->capacity; i++) {
		if (table->values[i].key) {
			free(table->values[i].key);
		}
	}
	free(table->values);
	free(table);
}

struct hashmap *clone_and_double(struct hashmap *table)
{
	struct hashmap *new_table;
	new_table = hashmap_alloc(2 * table->capacity);
	struct hashmap_entry *entry;
	for (int i = 0; i < table->capacity; i++) {
		entry = table->values + i;
		if (entry->key) {
			hashmap_put(new_table, entry->key, entry->value);
		}
	}
	return new_table;
}

/**
 * @brief Insert value into hash table.
 * @detail Insert using quadratic probing as collision strategy.
 * Double capacity when size > (0.7 * capacity).
 */
struct hashmap *hashmap_put(struct hashmap *table, const char *key, int value)
{
	if (table->size > 0.7 * table->capacity) {
		struct hashmap *doubled_table = clone_and_double(table);
		hashmap_free(table);
		table = doubled_table;
	}

	unsigned long hashval = hash((char *)key);
	unsigned long position;
	unsigned char reassign = 0;
	int i = 1;
	do {
		// Quadratic probing
		position = hashval + ((int)(0.5 * i)) + ((int)(0.5 * i * i));
		position %= table->capacity;
		i++;
	} while (table->values[position].key
		 && !(reassign =
		      strcmp(table->values[position].key, key) == 0));

	if (!reassign) {
		table->size++;
		table->values[position].key = malloc(strlen(key) + 1);
		strcpy(table->values[position].key, key);
	}

	table->values[position].value = value;
	return table;
}

/**
 * @brief Get value from hash table.
 * @detail 
 */
int hashmap_get(struct hashmap *table, const char *key)
{
	unsigned long hashval = hash((char *)key);
	unsigned long position;
	int i = 1;
	do {
		// Quadratic probing
		position = hashval + ((int)(0.5 * i)) + ((int)(0.5 * i * i));
		position %= table->capacity;
		i++;
	} while (!table->values[position].key
		 || strcmp(table->values[position].key, key) != 0);
	return table->values[position].value;
}

int main(int argc, const char *argv[])
{
    puts("Populate hashmap");
	struct hashmap *table = hashmap_alloc(4);
	for (int i = 0; i < 2048; i++) {
		char key[10];
		sprintf(key, "key%i", i);
		table = hashmap_put(table, key, i);
	}

    puts("Check correct mappings");
    int errors = 0;
	for (int i = 0; i < 2048; i++) {
		char key[10];
		sprintf(key, "key%i", i);
		if (hashmap_get(table, key) != i) {
            errors++;
		}
	}

	hashmap_free(table);

    printf("Errors: %i\n", errors);
	return 0;
}
