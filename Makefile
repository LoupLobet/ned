CC=cc

CFLAGS=-Wall -pedantic -std=c99
LDFLAGS=-lncurses

PROG=ned
OBJS=\
	util.o\
	buffer.o\
	rune.o\
	file.o\
	main.o

all: clean $(PROG)

clean:
	rm -f *.o

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $<

# utf/
rune.o: utf/rune.c
	$(CC) -c $^ $(CFLAGS)
