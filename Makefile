CC=gcc -std=c99 -I. $(CFLAGS)
CFLAGS=-O3 -g -Wall -pedantic
LINK=gcc

LIBS=-lm
GUI_LIBS=-lm -lGL -lGLU `sdl-config --cflags --libs`
GUI_EXE=glplanets

GUI_OBJS=gui/guimain.o gui/events.o gui/draw.o

$(GUI_EXE): $(GUI_OBJS)
	$(LINK) $(GUI_OBJS) -o $(GUI_EXE) $(GUI_LIBS) 

.PHONY: clean
clean:
	-rm -rf $(EXE) $(OBJS) $(GUI_OBJS) *.d gui/*.d

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

