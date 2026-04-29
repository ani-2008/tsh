CC = gcc
TARGET = tsh 
SRC = src/tsh.c 

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) 
clean:
	rm -f $(TARGET)
