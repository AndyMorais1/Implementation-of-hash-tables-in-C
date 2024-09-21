#pragma once

#include <stdbool.h>
#include "list.h"

#define DEFAULT_SIZE 101

typedef struct _HashTable* HashTable;

// Creates a new hash table with the given size, hash function and key equality function. The hash function should return a number between 0 and size - 1. The key equality function should return true if the two keys are equal, false otherwise.
HashTable hash_table_create(int size, int (*hash)(void*, int), bool (*key_equal)(void*, void*));

// Destroys the hash table, freeing all the memory used by it. If key_destroy or value_destroy are not NULL, they will be called for each key and value in the hash table.
void hash_table_destroy(HashTable htable, void (*key_destroy)(void*), void (*value_destroy)(void*));

// Returns true if the hash table is empty, false otherwise.
bool hash_table_is_empty(HashTable htable);

// Returns the number of elements in the hash table.
int hash_table_size(HashTable htable);

// Inserts a new key-value pair in the hash table. If the key already exists, the value will be updated. Returns NULL if the key is new, or the old value if the key already exists.
void* hash_table_insert(HashTable htable, void* key, void* value);

// Removes the key-value pair with the given key from the hash table and returns the value.
void* hash_table_remove(HashTable htable, void* key);

// Returns the value associated with the given key, or NULL if the key is not in the hash table.
void* hash_table_get(HashTable htable, void* key);

// Returns a linked list with all the keys in the hash table.
List hash_table_keys(HashTable htable);

// Returns a linked list with all the values in the hash table.
List hash_table_values(HashTable htable);

// Rehashes the hash table to a new size. If key_destroy or value_destroy are not NULL, they will be called for each key and value in the hash table. If new_size is less than or equal to 0, no rehashing will be done.
HashTable hash_table_rehash(HashTable htable, int new_size, void (*key_destroy)(void*), void (*value_destroy)(void*));
