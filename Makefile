CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g

CFILES=$(wildcard *.c)
OFILES=$(addprefix obj/, $(patsubst %.c, %.o, $(CFILES)))

BIN=lc

$(BIN): $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -o $(BIN)

obj/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OFILES)
	rm -rf $(BIN)
