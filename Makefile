CC=gcc
TARGET = slides
CFLAGS=-std=c11 -O3 -Wall
LIBS=-lncurses

all:
	$(CC) $(CFLAGS)  slides.c $(LIBS) -o $(TARGET)

clean:
	-rm -f $(TARGET)