#pragma once
#include <iostream>

const int BigIntBase = 1000000;
class BigInt{
	bool isNegative;
	size_t size;
	size_t maxsize;
	int* line;
	void push(int n);
public:
	bool getSign() const;
	size_t getSize() const;
	size_t getMaxSize() const;
	~BigInt();
	const int& operator[](size_t i) const;
	int& operator[](size_t i);
	BigInt operator-() const;
	bool operator==(const BigInt& other) const;
	bool operator!=(const BigInt& other) const;
	bool operator<(const BigInt& other) const;
	bool operator>(const BigInt& other) const;
	bool operator>=(const BigInt& other) const;
	bool operator<=(const BigInt& other) const;

	friend BigInt operator+(const BigInt &A, const BigInt &B);
	friend BigInt operator-(const BigInt &A, const BigInt &B);

	friend std::ostream& operator<<(std::ostream& out, const BigInt& bi);

	BigInt(int n);
	BigInt(const BigInt& B);

	BigInt& operator=(const BigInt& copied);
	BigInt(BigInt&& moved);
	BigInt& operator=(BigInt&& moved);
};
BigInt operator-(const BigInt &A, const BigInt &B);
std::ostream& operator<<(std::ostream& out, const BigInt& A);
BigInt operator+(const BigInt &A, const BigInt &B);