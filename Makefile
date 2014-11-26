ODIR := obj/
SDIR := src/
LDFLAGS = -lncurses
#TODO fix -g
DFLAGS = -g -std=c99 -Wall -Wextra -pedantic
CC = clang

lz : $(ODIR)lz.o $(ODIR)edist.o
	$(CC) $^ -o $@ $(LDFLAGS)

$(ODIR)lz.o : $(SDIR)$(wildcard lz.[ch])
	$(CC) $(DFLAGS) -c $(SDIR)lz.c -o $@

$(ODIR)edist.o : $(SDIR)$(wildcard edist.[ch])
	$(CC) $(DFLAGS) -c $(SDIR)edist.c -o $@

$(ODIR):
	mkdir -p $(ODIR)

clean :
	-rm -f $(ODIR)*
	-rm lz

.PHONY : clean
