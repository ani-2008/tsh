CC = gcc
TARGET = tsh 
SRC = src/tsh.c 

all:
	$(CC) $(SRC) -o $(TARGET) 
clean:
	rm -f $(TARGET)
