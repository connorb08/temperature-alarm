CC = gcc
CFLAGS = -O2 -Wall
LFLAGS =


all:	controller

###

read_temp.o:	read_temp.c read_temp.h
	$(CC) $(CFLAGS) -c read_temp.c

###

write_display.o:	write_display.c write_display.h
	$(CC) $(CFLAGS) -c write_display.c

###

controller:	controller.o read_temp.o write_display.o
	$(CC) -o controller controller.o read_temp.o write_display.o $(LFLAGS)

controller.o:	controller.c read_temp.h write_display.h
	$(CC) $(CFLAGS) -c controller.c

clean:	
	rm -f *~ *.o controller