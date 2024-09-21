SRC=src
BIN=bin
TESTS_SRC=test
TESTS_BIN=bin/test
CFLAGS=-g
# CFLAGS=-Wall -Wextra -Werror -std=c11 -g

all: setup $(TESTS_BIN)/test_open_hash_table

setup:
	mkdir -p bin/test

$(TESTS_BIN)/test_open_hash_table: $(TESTS_SRC)/test_open_hash_table.c $(SRC)/open_hash_table.c $(SRC)/singly_linked_list.c $(TESTS_BIN)/unity.o
	$(CC) $(CFLAGS) -o $@ $^

$(TESTS_BIN)/unity.o: $(TESTS_SRC)/unity/unity.c
	$(CC) -c $(CFLAGS) -o $@ $<

test: all
	$(TESTS_BIN)/test_open_hash_table

clean: 
	rm -rf $(BIN) $(TESTS_BIN)
