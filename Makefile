OBJDIR := obj
SRCDIR := src
LDFLAGS = -lncurses
#TODO fix -g
DFLAGS = -g -std=c99 -Wall -Wextra -pedantic
OBJECTS = $(addprefix $(OBJDIR)/, lz.o edist.o)
CC = clang

lz : $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/lz.o : $(SRCDIR)/$(wildcard lz.[ch])
	$(CC) $(DFLAGS) -c $(SRCDIR)/lz.c -o $@

$(OBJDIR)/edist.o : $(SRCDIR)/$(wildcard edist.[ch])
	$(CC) $(DFLAGS) -c $(SRCDIR)/edist.c -o $@

$(OBJECTS) : $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean :
	-rm -f $(OBJDIR)/*
	-rm lz

.PHONY : clean
