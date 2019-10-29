CC=g++

all: MyTest Test_from_mtrempoltsev_github

MyTest: main.cpp my_matrix.h
	$(CC) -o MyTest main.cpp
	
Test_from_mtrempoltsev_github: NotMyTest.cpp my_matrix.h
	$(CC) -o Test_from_mtrempoltsev_github NotMyTest.cpp
	
clean:
	rm -rf *.o test run