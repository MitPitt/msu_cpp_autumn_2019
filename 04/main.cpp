#include <iostream>
#include "my_matrix.h"

int main()
{
	const size_t rows = 5;
	const size_t cols = 3;
	Matrix a = Matrix(rows, cols);
	
	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < cols; j++)
			a[i][j] = (i + 1)*(j + 1);

	a[1][1] = 0;

	a.print();

	//Testing out of bounds

	try { a[6][6] = 0; }
	catch (std::out_of_range& error) {
		std::cerr << error.what() << '\n';
	}

	try { a[1][6] = 0; }
	catch (std::out_of_range& error) {
		std::cerr << error.what() << '\n';
	}
	
	//Testing operator==

	std::cout << "(a==a)=" << (a == a) << '\n';

	Matrix b = Matrix(rows, cols);

	std::cout << "(a==b)=" << (a == b) << '\n'; 
	
	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < cols; j++)
			b[i][j] = (i + 1)*(j + 1);

	b[1][1] = 0;

	std::cout << "(a==b)=" << (a == b) << '\n';
	
	Matrix c = Matrix(2,2);
	std::cout << "(a==c)=" << (a == c) << '\n';
	
	//Testing operator*=
	
	std::cout << "Matrix a before:"<< '\n';
	a.print();
	a *= 3;
	std::cout << "Matrix a after *=3:"<< '\n';
	a.print();
	
}