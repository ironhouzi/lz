OBJDIR := obj
SRCDIR := src
LDFLAGS = -lncurses
#TODO fix -g
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g
OBJECTS = $(addprefix $(OBJDIR)/, lz.o edist.o)
CC = clang

lz : $(ODIR)lz.o $(ODIR)edist.o $(ODIR)dynarr.o
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/lz.o : $(SRCDIR)/$(wildcard lz.[ch])
	$(CC) $(CFLAGS) -c $(SRCDIR)/lz.c -o $@

$(OBJDIR)/edist.o : $(SRCDIR)/$(wildcard edist.[ch])
	$(CC) $(CFLAGS) -c $(SRCDIR)/edist.c -o $@

$(ODIR)dynarr.o : $(SDIR)$(wildcard dynarr.[ch])
	$(CC) $(DFLAGS) -c $(SDIR)dynarr.c -o $@

$(ODIR):
	mkdir -p $(ODIR)

clean :
	-rm -f $(OBJDIR)/*
	-rm lz

.PHONY : clean
