CC := /usr/bin/gcc
CCFLAGS = -Wall -std=gnu11
DBGFLAGS = -g -O0
LDFLAGS =
LDLIBS =
INCLUDES = 

all: crc

crc: crc.o crclib.o
	$(CC) $(CCFLAGS) $(DBGFLAGS) $(INCLUDES) $(LDLIBS) $(LDFLAGS) -o crc crc.o crclib.o 

crc.o: crc.c crclib.h
	$(CC) $(CCFLAGS) $(DBGFLAGS) $(INCLUDES) $(LDLIBS) $(LDFLAGS) -c crc.c

crclib.o: crclib.c crclib.h
	$(CC) $(CCFLAGS) $(DBGFLAGS) $(INCLUDES) $(LDLIBS) $(LDFLAGS) -c crclib.c

dist: crc.c crclib.c crclib.h
	$(CC) $(CCFLAGS) $(INCLUDES) -O3 $(LDLIBS) $(LDFLAGS) -o crc crc.c crclib.c 

distclean: crc.c crclib.c crclib.h
	rm crc *.o
	$(CC) $(CCFLAGS) $(INCLUDES) -O3 $(LDLIBS) $(LDFLAGS) -o crc crc.c crclib.c 

clean:
	rm crc *.o
