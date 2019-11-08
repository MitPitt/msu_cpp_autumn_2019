CC=g++

all: test

test: main.o BigInt.o
	$(CC) -o test main.o BigInt.o
	
main.o: main.cpp BigInt.h
	$(CC) -c main.cpp
	
BigInt.o: BigInt.cpp BigInt.h
	$(CC) -c BigInt.cpp

clean:
	rm -rf *.o test