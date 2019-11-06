#include <iostream>
#include <algorithm>
#include <iomanip>

const int BigIntBase = 1000000;
class BigInt {
	bool isNegative;
	size_t size;
	size_t maxsize;
	int* line;
	void push(int n) {
		line[size] = n;
		size += 1;
		if (size >= maxsize) {
			int* newline = new int[maxsize * 2];
			maxsize *= 2;
			for (size_t i = 0; i < size; i++)
				newline[i] = line[i];
			delete[] line;
			line = newline;
		}
	}
public:
	bool getSign() const {
		return isNegative;
	}
	size_t getSize() const {
		return size;
	}
	size_t getMaxSize() const {
		return maxsize;
	}
	~BigInt() {
		delete[] line;
	}
	const int& operator[](size_t i) const {
		return line[i];
	}
	int& operator[](size_t i) {
		return line[i];
	}
	BigInt operator-() const {
		BigInt tmp(*this);
		tmp.isNegative = !isNegative;
		return tmp;
	}
	bool operator==(const BigInt& other) const {
		if (this == &other)
			return true;
		if (size != other.size) return false;
		for (size_t i = 0; i < size; ++i)
			if (line[i] != other[i])
				return false;
		return true;
	}
	bool operator!=(const BigInt& other) const {
		return !(*this == other);
	}
	bool operator<(const BigInt& other) const {
		bool rev = false;
		if (this == &other)
			return false;
		if ((isNegative) && !(other.getSign())) return true;
		else if (!(isNegative) && (other.getSign())) return false;
		else if ((isNegative) && (other.getSign())) rev = true;
		bool ans = false;
		if (size < other.getSize()) ans = true;
		else if (size > other.getSize()) ans = false;
		else
			for (size_t i = 0; i < size; ++i)
				if (line[i] > other[i]) { ans = false; break; }
				else if (line[i] < other[i]) { ans = true; break; }
		return (ans != rev);
	}
	bool operator>(const BigInt& other) const {
		bool rev = false;
		if (this == &other)
			return false;
		if ((isNegative) && !(other.getSign())) return false;
		else if (!(isNegative) && (other.getSign())) return true;
		else if (isNegative && other.getSign()) rev = true;
		bool ans = false;
		if (size > other.getSize()) ans = true;
		else if (size < other.getSize()) ans = false;
		else
			for (size_t i = 0; i < size; ++i)
				if (line[i] < other[i]) { ans = false; break; }
				else if (line[i] > other[i]) { ans = true; break; }
		return (ans != rev);
	}
	bool operator>=(const BigInt& other) const {
		return !(*this < other);
	}
	bool operator<=(const BigInt& other) const {
		return !(*this > other);
	}

	friend BigInt operator+(const BigInt &A, const BigInt &B);
	friend BigInt operator-(const BigInt &A, const BigInt &B);

	friend std::ostream& operator<<(std::ostream& out, const BigInt& bi);

	BigInt(int n);
	BigInt(const BigInt& B);

	BigInt& operator=(const BigInt& copied)
	{
		if (this == &copied)
			return *this;
		int* ptr = new int[copied.maxsize];
		delete[] line;
		line = ptr;
		isNegative = copied.isNegative;
		size = copied.size;
		maxsize = copied.maxsize;
		std::copy(copied.line, copied.line + maxsize, line);
		return *this;
	}
	BigInt(BigInt&& moved) : line(moved.line), maxsize(moved.maxsize), size(moved.size), isNegative(moved.isNegative) {
		moved.line = nullptr;
		moved.maxsize = 0;
		moved.size = 0;
	}
	BigInt& operator=(BigInt&& moved) {
		if (this == &moved)
			return *this;
		delete[] line;
		isNegative = moved.isNegative;
		line = moved.line;
		size = moved.size;
		maxsize = moved.maxsize;
		moved.line = nullptr;
		moved.maxsize = 0;
		moved.size = 0;
		return *this;
	}
};

BigInt::BigInt(int n = 0) : size(0), maxsize(8), isNegative(false) {

	line = new int[8];

	if (n < 0) {
		isNegative = true;
		n = -n;
	}
	while (n >= BigIntBase) {
		push(n % BigIntBase);
		n = n / BigIntBase;
	}
	line[0] = 0;
	if (n != 0)
		push(n);

}

BigInt::BigInt(const BigInt& B) : maxsize(B.maxsize), size(B.size), isNegative(B.isNegative) {

	line = new int[maxsize];

	for (size_t i = 0; i < size; i++) {
		line[i] = B.line[i];
	}

}

BigInt operator+(const BigInt &A, const BigInt &B) {
	bool neg = false;
	if (A.isNegative) {
		if (B.isNegative) {
			neg = true;
		}
		else {
			return B - (-A);
		}
	}
	else if (B.isNegative) {
		return A - (-B);
	}

	BigInt res;

	int overflow = 0;
	size_t m = std::min(A.size, B.size);


	for (size_t i = 0; i < m; i++) {
		int newn = A.line[i] + B.line[i] + overflow;
		if (newn >= BigIntBase) {
			newn -= BigIntBase;
			overflow = 1;
		}
		else
			overflow = 0;
		res.push(newn);
	}
	if (A.size > B.size)
		for (size_t i = m; i < A.size; i++) {
			int newn = A.line[i] + overflow;
			if (newn >= BigIntBase) {
				newn -= BigIntBase;
				overflow = 1;
			}
			res.push(newn);
		}
	else if (A.size < B.size)
		for (size_t i = m; i < B.size; i++) {
			int newn = B.line[i] + overflow;
			if (newn >= BigIntBase) {
				newn -= BigIntBase;
				overflow = 1;
			}
			res.push(newn);
		}
	if (overflow == 1)
		res.push(1);
	if (neg)
		res.isNegative = !res.isNegative;

	return res;
}

BigInt operator-(const BigInt &A, const BigInt &B) {
	if (B.isNegative) {
		return A + (-B);
	}
	else if ((A.isNegative) || (A < B)) {
		return -(B - A);
	}

	BigInt res = A;


	for (int i = 0; i < res.size; i++) {
		int minus = 0;
		if (i < B.size)
			minus = B.line[i];
		res.line[i] -= minus;
		if (res.line[i] < 0) {
			res.line[i] += BigIntBase;
			res.line[i + 1]--;
		}
	}

	size_t pos = res.size;
	while (pos && !res.line[pos]) {
		pos--;
	}
	res.size = pos;

	return res;
}

std::ostream& operator<<(std::ostream& out, const BigInt& A) {
	if (A.isNegative) {
		out << "-";
	}
	if (A.size == 0)
		out << "0";
	else {
		out << A[A.size - 1];
		for (size_t i = 1; i < A.size; i++)
			out << std::setfill('0') << std::setw(6) << A[A.size - 1 - i];
	}
	return out;
}