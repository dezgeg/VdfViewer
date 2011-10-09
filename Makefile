CC=gcc -std=c99 $(CFLAGS)
CFLAGS=-O0 -g -Wall -Werror -pedantic
LINK=gcc

LIBS=-lm
EXE=planets
OBJS=main.o planet.o simulate.o
TEST_OBJS=foo.o

$(EXE): $(OBJS)
	$(LINK) -o $(EXE) $(OBJS) $(LIBS)

main.c: planet.h

planet.c: planet.h vector.h

simulate.c: vector.h

.PHONY: clean
clean:
	rm -rf $(EXE) $(OBJS)
