CC = gcc
CFLAGS = -Wall
TARGET = bin/tsh 
SRC = src/tsh.c 

all:
	mkdir -p bin/
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) 
clean:
	rm -f $(TARGET)
