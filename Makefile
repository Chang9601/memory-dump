CC=gcc
CFLAGS=-g -Wall -Wshadow -Wvla -Werror -pedantic -Wno-unused-function
DEBUG=gdb
TOOL=valgrind
SRC_C=memory-dump.c
OBJ_O=memory-dump.o
EXECUTABLE=memory-dump.exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_O)
	$(CC) $(CFLAGS) $(OBJ_O) -o $@

$(OBJ_O): $(SRC_C)
	$(CC) -c $(CFLAGS) $< -o $@

debug:
	$(DEBUG) $(EXECUTABLE)

clean:
	rm -f $(OBJ_O) $(EXECUTABLE)

run: $(EXECUTABLE)
	$(TOOL) ./$(EXECUTABLE)