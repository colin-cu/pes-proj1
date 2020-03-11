CC=gcc
CFLAGS=-Wall -Werror
OBJ = prog1.o

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

prog1: $(OBJ) 
	$(CC) -o $@ $^ $(CFLAGS)
