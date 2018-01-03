# Makefile

CC = g++
BIN = a.out

SOURCES = Display.cpp Chip8.cpp main.cpp OpFuncs.cpp

CFLAGS = 
INCDIRS = 
LIBDIRS = 
LIBS = -lSDL2

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(BIN) $(INCDIRS) $(LIBDIRS) $(LIBS) 
clean:
	rm -rf $(BIN)
