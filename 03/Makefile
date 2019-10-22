CC=g++

all: run test

test: run test.py
	python test.py

run: main.cpp
	$(CC) -o run main.cpp
	
clean:
	rm -rf *.o test run