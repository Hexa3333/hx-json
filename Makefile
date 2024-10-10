CC=gcc
SRC=$(wildcard src/*.c)
CFLAGS=-g

all:
	$(CC) $(SRC) $(CFLAGS)
