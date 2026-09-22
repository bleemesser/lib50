CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -Iinclude
AR = ar # bundler for static .a
ARFLAGS = rcs # make/update the archive, create if it doesn't exist, write an index

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o) # replace .c with .o

lib50.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

src/intops.o: include/lib50/intops.h

test: tests/test_intops
	./tests/test_intops

tests/test_intops: tests/test_intops.c lib50.a
	$(CC) $(CFLAGS) -o $@ tests/test_intops.c lib50.a

clean:
	rm -f lib50.a src/*.o tests/test_intops

.PHONY: test clean
