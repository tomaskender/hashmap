CC=gcc
CFLAGS= -std=c99 -Wextra -Wall -pedantic -g

CXX=g++
CXXFLAGS= -std=c++11 -Wextra -Wall -pedantic -g

EXECS= tail tail2 wordcount wordcount-dynamic

#LIB_SRCS_OLD= htab_begin.c htab_bucket_count.c htab_clear.c htab_end.c htab_free.c htab_hash.c htab_init.c htab_iterator_get_key.c htab_iterator_get_value.c htab_iterator_next.c htab_iterator_set_value.c htab_lookup_add.c htab_lookup_count.c htab_move.c htab_size.c

LIB_SRCS= $(shell ls | grep htab_.*\.c | tr "\n" " ")
LIB_OBJ_STATIC= $(patsubst %.c, bin/static/%.o, $(LIB_SRCS))
LIB_OBJ_SHARED= $(patsubst %.c, bin/shared/%.o, $(LIB_SRCS))

.PHONY: all clean zip

all: $(EXECS)

clean:
	rm -rf bin *.o $(EXECS)

zip:
	zip xkende01.zip Makefile *.h *.c *.cc

#-----------------------------------------#

tail: tail.o

tail2: tail2.o
	$(CXX) $(CFLAGS) $< -o $@

wordcount: bin/static/libhtab.a wordcount.o io.o htab.o
	$(CC) wordcount.o io.o htab.o -Lbin/static -lhtab -o wordcount

wordcount-dynamic: bin/shared/libhtab.so wordcount.o io.o htab.o
	$(CC) wordcount.o io.o htab.o -Lbin/shared -lhtab -o wordcount-dynamic

bin/static/libhtab.a: $(LIB_OBJ_STATIC)
	ar rcs bin/static/libhtab.a $(LIB_OBJ_STATIC)

bin/shared/libhtab.so: $(LIB_OBJ_SHARED)
	gcc -shared $(LIB_OBJ_SHARED) -o bin/shared/libhtab.so

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

bin/shared/%.o: %.c htab.h htab_definition.h
	mkdir -p bin/shared
	$(CC) $(CFLAGS) -fPIC -o $@ -c $<

bin/static/%.o: %.c htab.h htab_definition.h
	mkdir -p bin/static
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.c htab.h
	$(CC) $(CFLAGS) -o $@ -c $<
