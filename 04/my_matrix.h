class Row {
	int* row;
	size_t len;
public:
	Row(int* start, size_t len) : len(len), row(start) {}
	const int& operator[](size_t pos) const;
	int& operator[](size_t pos);
};

const int& Row::operator[](size_t pos) const {
	if (pos >= len)
		throw std::out_of_range("col out of range");
	return row[pos];
}

int& Row::operator[](size_t pos) {
	if (pos >= len)
		throw std::out_of_range("col out of range");
	return row[pos];
}


class Matrix {
	size_t rows, cols;
public:
	int* matr;
	Matrix(size_t n, size_t m, int def = 0) : rows(n), cols(m) { matr = new int[n*m]; }
	size_t getRows() const { return rows; }
	size_t getColumns() const { return cols; }

	Matrix& operator*=(int num);
	bool operator==(const Matrix& comp) const;
	bool operator!=(const Matrix& comp) const;

	const Row operator[](size_t rownum) const;
	Row operator[](size_t rownum);
	void print();
};

Matrix& Matrix::operator*=(int num) {
	for (size_t i = 0; i < rows*cols; i++)
		matr[i] *= num;
	return *this;
}
bool Matrix::operator==(const Matrix& comp) const {
	if (&comp == this)
		return true;
	else if ((cols == comp.getColumns()) && (rows == comp.getRows())) {
		for (size_t i = 0; i < rows; i++)
			for (size_t j = 0; j < cols; j++)
				if ((*this)[i][j] != comp[i][j]) ////////////
					return false;
		return true;
	}
	else
		return false;
}
bool Matrix::operator!=(const Matrix& comp) const {
	return !(*this == comp);
}
const Row Matrix::operator[](size_t rownum) const {
	if (rownum >= rows)
		throw std::out_of_range("row out of range");
	return Row(matr + rownum * cols, cols);
}
Row Matrix::operator[](size_t rownum) {
	if (rownum >= rows)
		throw std::out_of_range("row out of range");
	return Row(matr + rownum * cols, cols);
}
void Matrix::print(){
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++)
			std::cout << (*this)[i][j] << ' ';
		std::cout << '\n';
	}
}