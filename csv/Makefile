CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -O2 -g
TARGET=zone-summary
SRC=main.c csv.c zoneSummary.c
OBJ=$(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean