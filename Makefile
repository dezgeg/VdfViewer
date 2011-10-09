CC=gcc -std=c99 $(CFLAGS)
CFLAGS=-O0 -g -Wall -Werror -pedantic
LINK=gcc

LIBS=-lm
EXE=planets
OBJS=planet.o main.o
TEST_OBJS=foo.o

$(EXE): $(OBJS)
	$(LINK) -o $(EXE) $(OBJS) $(LIBS)

main.c: planet.h

planet.c: planet.h

.PHONY: clean
clean:
	rm -rf $(EXE) $(OBJS)
