CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -O2 -g
TARGET=wow-randomizer
SRC=src/main.c src/spawnsCSV.c
OBJ=$(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean