CC=gcc
CFLAGS=-Wall
OBJECTS=main.o Menu.o TypeMenuElement.o

palmtop: $(OBJECTS)
	$(CC) $(CFLAGS) -o palmtop $(OBJECTS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c
Menu.o: Menu.c
	$(CC) $(CFLAGS) -c Menu.c
TypeMenuElement.o: TypeMenuElement.c
	$(CC) $(CFLAGS) -c TypeMenuElement.c

.PHONY: clean

clean:
	rm -f palmtop main.o Menu.o TypeMenuElement.o
