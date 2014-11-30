OBJDIR := obj
SRCDIR := src
LDFLAGS = -lncurses
#TODO fix -g
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g
OBJECTS = $(addprefix $(OBJDIR)/, lz.o edist.o dynarr.o)
CC = clang

lz : $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/lz.o : $(SRCDIR)/$(wildcard lz.[ch])
	$(CC) $(CFLAGS) -c $(SRCDIR)/lz.c -o $@

$(OBJDIR)/edist.o : $(SRCDIR)/$(wildcard edist.[ch])
	$(CC) $(CFLAGS) -c $(SRCDIR)/edist.c -o $@

$(OBJDIR)/dynarr.o : $(SRCDIR)/$(wildcard dynarr.[ch])
	$(CC) $(CFLAGS) -c $(SRCDIR)/dynarr.c -o $@

$(OBJECTS) : $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean :
	-rm -f $(OBJDIR)/*
	-rm lz

.PHONY : clean
