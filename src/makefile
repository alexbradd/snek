CC = gcc
CFLAGS = -I. -Wall -O2

TARGET = snek

$(TARGET): main.o input.o graphics.o
	$(CC) $(CFLAGS) main.o input.o graphics.o -o $(TARGET)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

input.o: input.c input.h
	$(CC) $(CFLAGS) -c input.c

graphics.o: graphics.c graphics.h
	$(CC) $(CFLAGS) -c graphics.c

clean:
	$(RM) *.o $(TARGET)
