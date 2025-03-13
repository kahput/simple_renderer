#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HASH_BIT 6

typedef struct hash_table HashTable;

HashTable *hashtable_create(size_t key_size, size_t value_size);
void hashtable_clear(HashTable *ht);
void hashtable_free(HashTable *ht);

bool hashtable_insert(HashTable *ht, const char *key, void *value);
bool hashtable_remove(HashTable *ht, const char *key);

void *hashtable_get(HashTable *ht, const char *key);

size_t hashtable_size(HashTable *ht);
size_t hashtable_capacity(HashTable *ht);
