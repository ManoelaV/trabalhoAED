CC = gcc
CFLAGS = -Wall -g

all: main.o poketrunfo.o
	$(CC) $(CFLAGS) -o all main.o poketrunfo.o

main.o: main.c poketrunfo.h
	$(CC) $(CFLAGS) -c main.c

poketrunfo.o: poketrunfo.c poketrunfo.h
	$(CC) $(CFLAGS) -c poketrunfo.c

clean:
ifeq ($(OS),Windows_NT)
	del /F /Q *.o *~ all.exe
else
	rm -f *.o *~ all
endif