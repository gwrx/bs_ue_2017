CC=gcc
CFLAGS=-I. -std=c99 -Wall -Werror -D_POSIX_C_SOURCE=199309L
DEPS = bs_exam2_task2.h
OBJ = $(DEPS:.h=.o)
LDLIBS = -lpthread

.PHONY: all clean
all: main
	
main: main.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)

clean:
	rm -f *.o main
	
