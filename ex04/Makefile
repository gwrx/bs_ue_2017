CFLAGS = -Wall -Werror -std=c99
%: %.c
LDLIBS = -lm

SRCS = $(wildcard *.c)

ALL: $(patsubst %.c,%,$(SRCS)) $(LDLIBS)

