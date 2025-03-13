#include "hash_table.h"
#include <stdlib.h>

struct hash_table_node {
};

struct hash_table {
	struct hash_table_node **buckets[64];
	uint32_t split, count;
};

HashTable *hashtable_create(size_t key_size, size_t value_size) {
	struct hash_table *ht = malloc(sizeof(struct hash_table));
}

void hashtable_clear(HashTable *ht);
void hashtable_free(HashTable *ht);
