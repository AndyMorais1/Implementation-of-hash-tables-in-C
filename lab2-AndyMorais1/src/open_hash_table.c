#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "list.h"
#include "hash_table.h"

typedef struct _HashTable *HashTable;
typedef struct _Item *Item;

struct _HashTable
{
    int size;
    List *table;
    int (*hash)(void *, int);
    bool (*key_equal)(void *, void *);
};

struct _Item
{
    void *key;
    void *value;
};

#define DEFAULT_SIZE 101

bool default_key_equal(void *str1, void *str2)
{
    return strcmp((char *)str1, (char *)str2) == 0;
}

int default_hash(void *key, int n)
{
    int result = 0, a = 127;
    for (int i = 0; ((char *)key)[i] != '\0'; i++)
    {
        result = (result * a + ((char *)key)[i]);
    }
    return result % n;
}

Item item_create(void *key, void *value)
{
    Item item = malloc(sizeof(struct _Item));
    item->key = key;
    item->value = value;
    return item;
}

void item_destroy(Item item)
{
    free(item);
}

HashTable hash_table_create(int size, int (*hash)(void *, int), bool (*key_equal)(void *, void *))
{
    HashTable ht = malloc(sizeof(struct _HashTable));
    ht->size = size == -1 ? DEFAULT_SIZE : size;
    ht->hash = hash == NULL ? default_hash : hash;
    ht->key_equal = key_equal == NULL ? default_key_equal : key_equal;
    ht->table = malloc(sizeof(List) * ht->size);
    for (int i = 0; i < ht->size; i++)
    {
        ht->table[i] = list_create();
    }
    return ht;
}

void hash_table_destroy(HashTable htable, void (*key_destroy)(void *), void (*value_destroy)(void *))
{
    for (int i = 0; i < htable->size; i++)
    {
        List list = htable->table[i];
        while (!list_is_empty(list))
        {
            Item item = list_remove(list, 0);
            if (key_destroy != NULL)
                key_destroy(item->key);
            if (value_destroy != NULL)
                value_destroy(item->value);
            item_destroy(item);
        }
        list_destroy(list, NULL);
    }
    free(htable->table);
    free(htable);
}

bool hash_table_is_empty(HashTable htable)
{
    for (int i = 0; i < htable->size; i++)
    {
        if (!list_is_empty(htable->table[i]))
        {
            return false;
        }
    }
    return true;
}

int hash_table_size(HashTable htable)
{
    int count = 0;
    for (int i = 0; i < htable->size; i++)
    {
        List list = htable->table[i];
        int index = 0;
        while (list_get(list, index) != NULL)
        {
            count++;
            index++;
        }
    }
    return count;
}

void *hash_table_insert(HashTable htable, void *key, void *value)
{
    int idx = htable->hash(key, htable->size);
    List list = htable->table[idx];
    Item item = item_create(key, value);
    int list_idx = list_find(list, htable->key_equal, item);
    if (list_idx != -1)
    {
        Item old_item = list_get(list, list_idx);
        void *old_value = old_item->value;
        old_item->value = value;
        item_destroy(item);
        return old_value;
    }
    else
    {
        list_insert_last(list, item);
        return NULL;
    }
}

void *hash_table_remove(HashTable htable, void *key)
{
    int idx = htable->hash(key, htable->size);
    List list = htable->table[idx];
    Item item = item_create(key, NULL);
    int list_idx = list_find(list, htable->key_equal, item);
    item_destroy(item);
    if (list_idx != -1)
    {
        Item removed_item = list_remove(list, list_idx);
        void *value = removed_item->value;
        item_destroy(removed_item);
        return value;
    }
    return NULL;
}

void *hash_table_get(HashTable htable, void *key)
{
    int idx = htable->hash(key, htable->size);
    List list = htable->table[idx];
    Item item = item_create(key, NULL);
    int list_idx = list_find(list, htable->key_equal, item);
    item_destroy(item);
    if (list_idx != -1)
    {
        Item result = list_get(list, list_idx);
        return result->value;
    }
    return NULL;
}

List hash_table_keys(HashTable htable)
{
    List keys = list_create();
    for (int i = 0; i < htable->size; i++)
    {
        List list = htable->table[i];
        int index = 0;
        Item item = list_get(list, index);
        while (item != NULL)
        {
            list_insert_last(keys, item->key);
            index++;
            item = list_get(list, index);
        }
    }
    return keys;
}

List hash_table_values(HashTable htable)
{
    List values = list_create();
    for (int i = 0; i < htable->size; i++)
    {
        List list = htable->table[i];
        int index = 0;
        Item item = list_get(list, index);
        while (item != NULL)
        {
            list_insert_last(values, item->value);
            index++;
            item = list_get(list, index);
        }
    }
    return values;
}

HashTable hash_table_rehash(HashTable htable, int new_size, void (*key_destroy)(void *), void (*value_destroy)(void *))
{
    if (new_size <= 0)
    {
        return htable;
    }
    HashTable new_ht = hash_table_create(new_size, htable->hash, htable->key_equal);
    for (int i = 0; i < htable->size; i++)
    {
        List list = htable->table[i];
        int index = 0;
        Item item = list_get(list, index);
        while (item != NULL)
        {
            hash_table_insert(new_ht, item->key, item->value);
            index++;
            item = list_get(list, index);
        }
    }
    hash_table_destroy(htable, key_destroy, value_destroy);
    return new_ht;
}
