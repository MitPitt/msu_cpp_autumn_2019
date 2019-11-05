#include <iostream>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <sstream>
#include <math.h> 

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
			delete line;
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
		else if(!(isNegative) && (other.getSign())) return false;
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
		bool ans= false;
		if (size > other.getSize()) ans= true;
		else if (size < other.getSize()) ans= false;
		else
			for (size_t i = 0; i < size; ++i)
				if (line[i] < other[i]) { ans = false; break; }
				else if (line[i] > other[i]) { ans = true; break; }
		return (ans!=rev);
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
		int* ptr = new int[copied.size];
		delete[] line;
		line = ptr;
		isNegative = copied.isNegative;
		size = copied.size;
		maxsize = copied.maxsize;
		std::copy(copied.line, copied.line + size, line);
		return *this;
	}
	BigInt(BigInt&& moved) : line(moved.line), maxsize(moved.maxsize), size(moved.size), isNegative(moved.isNegative){
		moved.line = nullptr;
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
	if (n!=0)
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
	else if ((A.isNegative) || (A<B)) {
		return -(B - A) ;
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



std::string toString(const BigInt& value)
{
	std::stringstream buf;
	buf << value;
	return buf.str();
}
void check(int64_t x, int64_t y)
{
	const BigInt bigX = x;
	const BigInt bigY = y;
	if (bigX + bigY != BigInt(x + y))
	{
		std::cout << x << " + " << y << " != " << x + y << " got " << bigX + bigY << '\n';
	}

	if (bigX - bigY != BigInt(x - y))
	{
		std::cout << x << " - " << y << " != " << x - y << " got " << bigX - bigY << '\n';
	}
}
void doCheckEqual(const BigInt& actual, const char* expected, size_t line)
{
	const auto str = toString(actual);
	if (str != expected)
	{
		std::cout << "at line " << line << ": " << str << " != " << expected << '\n';
	}
}
#define checkEqual(x, y) do { doCheckEqual((x), (y), __LINE__); } while(0)
#define checkTrue(cond) do { if (!(cond)) std::cout << "at line " << __LINE__ << ": " << #cond << '\n'; } while(0)

int main()
{

	BigInt x = 3;
	checkEqual(x, "3");
	BigInt y = x;
	checkEqual(y, "3");
	BigInt z = 0;
	checkEqual(z, "0");

	checkEqual(BigInt(-10), "-10");

	checkTrue(x == y);
	checkTrue(y == x);
	checkTrue(x != z);
	checkTrue(z != x);

	z = y;
	checkEqual(z, "3");
	x = 100;
	checkEqual(x, "100");

	checkTrue(!(x < x));
	checkTrue(x < 200);
	checkTrue(BigInt(50) < x);
	checkTrue(BigInt(-500) < x);
	checkTrue(BigInt(-500) < BigInt(-200));

	checkTrue(!(x > x));
	checkTrue(BigInt(200) > x);
	checkTrue(x > BigInt(50));
	checkTrue(x > BigInt(-500));
	checkTrue(BigInt(-200) > BigInt(-500));

	checkTrue(x <= x);
	checkTrue(x <= 200);
	checkTrue(BigInt(50) <= x);
	checkTrue(BigInt(-500) <= x);
	checkTrue(BigInt(-500) <= BigInt(-200));

	checkTrue(x >= x);
	checkTrue(BigInt(200) >= x);
	checkTrue(x >= BigInt(50));
	checkTrue(x >= BigInt(-500));
	checkTrue(BigInt(-200) >= BigInt(-500));
	checkTrue(BigInt(0) == -BigInt(0));
	
	checkEqual(BigInt(100) - BigInt(100), "0");
	checkEqual(BigInt(99) - BigInt(100), "-1");
	checkEqual(BigInt(10) - BigInt(11), "-1");
	checkEqual(BigInt(20) - BigInt(19), "1");

	checkEqual(BigInt(10) + BigInt(10), "20");
	checkEqual(BigInt(-10) + BigInt(10), "0");
	checkEqual(BigInt(10) + BigInt(-10), "0");
	checkEqual(BigInt(-10) + BigInt(-10), "-20");
	
	checkEqual(BigInt(0) - BigInt(1), "-1");
	checkEqual(BigInt(0) + BigInt(-1), "-1");


	checkEqual(BigInt(10) - BigInt(10), "0");
	checkEqual(BigInt(-10) - BigInt(10), "-20");
	checkEqual(BigInt(10) - BigInt(-10), "20");
	checkEqual(BigInt(-10) - BigInt(-10), "0");


	for (int i = -21; i <= 21; ++i)
	{
		for (int j = -21; j <= 21; ++j)
		{
			check(i, j);
		}
	}

	
	BigInt a = 2;
	BigInt b = 1;
	for (int i=0; i < 40; i++) {
		b = b + b;
		a = a + a;
		std::cout << a << ' ' << b << '\n';
	}
	std::cout << "2^40 = " << int64_t(pow(2,40)) << '\n';

	std::cout << "a + b = " << a + b << '\n';

	std::cout << "done\n";

	return 0;
}
