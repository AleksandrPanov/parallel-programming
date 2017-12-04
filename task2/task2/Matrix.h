#pragma once
#include <vector>
class Matrix
{
private:
	std::vector<double> vv;
	int row;
	int col;
public:
	int gRow() const;
	int gCol() const;
	Matrix();
	Matrix(int row, int col);
	Matrix(int row, int col, double val);
	Matrix(int r, int c, double *p);

	double *getpA();
	double* operator [] (int i);
	Matrix operator * (Matrix& m);
	Matrix operator * (double val);
	Matrix operator / (double val);
	friend std::ostream& operator<<(std::ostream& os, Matrix& m);
};