# Simple Makefile for the lcc frontend.

ifndef SDCCLIB
SDCCLIB = /usr/lib/gbdk
endif

CC = gcc
CFLAGS = -O -Wall -DGBDKLIBDIR=\"$(SDCCLIB)\"
OBJ = lcc.o gb.o
BIN = lcc

all: $(BIN)

$(BIN): $(OBJ)

clean:
	rm -f *.o $(BIN) *~