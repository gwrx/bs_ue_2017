CFLAGS = -Wall -Wextra -Werror -std=c99 -O2

.PHONY: all clean

all: testvector

testvector: btree.o btree_test.o
	$(CC) $^ -o testvector

btree.o: btree.c btree.h
btree_test.o: btree_test.c btree.h

clean:
	$(RM) btree.o btree_test.o testvector
