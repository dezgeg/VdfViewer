CC=gcc -std=c99 $(CFLAGS)
CFLAGS=-O0 -g -Wall -Werror -pedantic
LINK=gcc

LIBS=-lm
EXE=planets
OBJS=main.o planet.o simulate.o
#TEST_OBJS=

$(EXE): $(OBJS)
	$(LINK) -o $(EXE) $(OBJS) $(LIBS)

.PHONY: clean
clean:
	-rm -rf $(EXE) $(OBJS) *.d

# Makefile autodependency crap generation
# stolen from http://scottmcpeak.com/autodepend/autodepend.html
-include $(OBJS:.o=.d)

%.o: %.c
	$(CC) -c $(CFLAGS) $*.c -o $*.o
	$(CC) -MM $(CFLAGS) $*.c > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

