CC = gcc
CFLAGS = -Wall -Werror -std=c99 -O2
OBJ = btree.o btree_test.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

btree: $(OBJ)
	gcc $(CFLAGS) -o $@ $^

all: $(OBJ)
	gcc $(CFLAGS) -o testvector $^

clean:
	rm $(OBJ)
