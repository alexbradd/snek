CC = gcc
CFLAGS = -I. -Wall -O2

TARGET = snek

$(TARGET): main.o input.o graphics.o
	$(CC) $(CFLAGS) main.o input.o graphics.o -o $(TARGET)

main.o:
	$(CC) $(CFLAGS) -c main.c

input.o:
	$(CC) $(CFLAGS) -c input.c

graphics.o:
	$(CC) $(CFLAGS) -c graphics.c

clean:
	$(RM) *.o $(TARGET)
