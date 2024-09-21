#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "unity/unity.h"

#include "../src/hash_table.h"

// Test table
HashTable table;

// Test data
char* keys[] = {"key0", "key1", "key2", "key3", "key4"};
char* values[] = {"value0", "value1", "value2", "value3", "value4"};
#define NITEMS 5

void setUp() {
    table = hash_table_create(-1, NULL, NULL);
}

void tearDown() {
    if (table != NULL) {
        hash_table_destroy(table, NULL, NULL);
    }
}

void test_insert_single_element() {
    TEST_ASSERT_TRUE(hash_table_is_empty(table));
    hash_table_insert(table, keys[0], values[0]);
    TEST_ASSERT_FALSE(hash_table_is_empty(table));
    void* retrieved = hash_table_get(table, "key0");
    TEST_ASSERT_NOT_NULL(retrieved);
    hash_table_remove(table, "key0");
    TEST_ASSERT_TRUE(hash_table_is_empty(table));
}

void test_insert_collection() {
    for (int i = 0; i < NITEMS; i++) {
        hash_table_insert(table, keys[i], values[i]);
    }
    TEST_ASSERT_EQUAL(values[3], hash_table_get(table, keys[3]));
    char* value = (char*)hash_table_remove(table, keys[3]);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(value, values[3], 6);
}

int custom_hash(void* key, int size) {
    return 0;
}

typedef struct {
    char a;
    char b;
} t_Key, *Key;

bool custom_key_equal(void* key1, void* key2) {
    Key k1 = (Key)key1;
    Key k2 = (Key)key2;
    return k1->a == k2->a && k1->b == k2->b;
}

void custom_key_destroy(void* key) {
    free(key);
}

typedef struct {
    int i;
    char c;
} t_Value, *Value;

void custom_value_destroy(void* value) {
    free(value);
}

void test_custom_table() {
    hash_table_destroy(table, NULL, NULL);
    table = hash_table_create(10, custom_hash, custom_key_equal);
    Key k0 = malloc(sizeof(t_Key));
    k0->a = 'a';
    k0->b = 'b';
    Value v0 = malloc(sizeof(t_Value));
    v0->i = 1;
    v0->c = 'a';
    Key k1 = malloc(sizeof(t_Key));
    k1->a = 'c';
    k1->b = 'd';
    Value v1 = malloc(sizeof(t_Value));
    v1->i = 2;
    v1->c = 'b';
    hash_table_insert(table, k0, v0);
    hash_table_insert(table, k1, v1);
    TEST_ASSERT_EQUAL(v0, hash_table_get(table, k0));
    TEST_ASSERT_EQUAL(v1, hash_table_get(table, k1));
    hash_table_destroy(table, custom_key_destroy, custom_value_destroy);
    table = NULL;
}

void test_update() {
    for (int i = 0; i < NITEMS; i++) {
        hash_table_insert(table, keys[i], values[i]);
    }
    char new_value[] = "new_value";
    void* old_value = hash_table_insert(table, "key3", new_value);
    TEST_ASSERT_EQUAL_STRING("value3", old_value);
    TEST_ASSERT_EQUAL_STRING("new_value", hash_table_get(table, "key3"));
}

void test_rehash() {
    hash_table_destroy(table, NULL, NULL);
    table = hash_table_create(3, NULL, NULL);
    hash_table_insert(table, keys[0], values[0]);
    hash_table_insert(table, keys[1], values[1]);
    hash_table_insert(table, keys[2], values[2]);
    TEST_ASSERT_EQUAL(3, hash_table_size(table));
    TEST_ASSERT_EQUAL(values[1], hash_table_get(table, "key1"));
    table = hash_table_rehash(table, 5, NULL, NULL);
    TEST_ASSERT_EQUAL(values[1], hash_table_get(table, "key1"));
    hash_table_insert(table, keys[3], values[3]);
    hash_table_insert(table, keys[4], values[4]);
    TEST_ASSERT_EQUAL(5, hash_table_size(table));
}

void test_key_collection() {
    for (int i = 0; i < NITEMS; i++) {
        hash_table_insert(table, keys[i], values[i]);
    }
    List key_list = hash_table_keys(table);
    TEST_ASSERT_EQUAL(NITEMS, list_size(key_list));
    for (int i = 0; i < NITEMS; i++) {
        TEST_ASSERT_EQUAL_STRING(keys[i], list_get(key_list, i));
    }
    list_destroy(key_list, NULL);
}

void test_value_collection() {
    for (int i = 0; i < NITEMS; i++) {
        hash_table_insert(table, keys[i], values[i]);
    }
    List value_list = hash_table_values(table);
    TEST_ASSERT_EQUAL(NITEMS, list_size(value_list));
    for (int i = 0; i < NITEMS; i++) {
        TEST_ASSERT_EQUAL_STRING(values[i], list_get(value_list, i));
    }
    list_destroy(value_list, NULL);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_insert_single_element);
    RUN_TEST(test_insert_collection);
    RUN_TEST(test_custom_table);
    RUN_TEST(test_update);
    RUN_TEST(test_rehash);
    RUN_TEST(test_key_collection);
    RUN_TEST(test_value_collection);
    return UNITY_END();
}