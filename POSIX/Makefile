CC=gcc
CFLAGS=-pedantic -std=c11 -Wall -D_POSIX_SOURCE -D_XOPEN_SOURCE

%.o: %.c common.h
	$(CC) -c -o $@ $< $(CFLAGS) 

all: lib_common.a barber client clean

lib_common.a: common.o
	ar rc $@ $^
	ranlib $@

barber: barber.o
	$(CC) -o $@ $< $(CFLAGS) -pthread -g lib_common.a -lrt

client: client.o
	$(CC) -o $@ $< $(CFLAGS) -pthread -g lib_common.a -lrt

clean:
	rm -f *.o
	
cleanAll:
	rm -f *.o
	rm -f client
	rm -f barber
