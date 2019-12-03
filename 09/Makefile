CC=g++

all: test run
	
testCPP: test.cpp Threads.h
	$(CC) -o testCPP test.cpp
	
testPY: test.cpp Threads.h
	$(CC) -o testCPP test.cpp
	
run:
	./test
	
clean:
	rm -rf *.o test