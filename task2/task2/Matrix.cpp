#include "Matrix.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int Matrix::gRow() const
{
	return row;
}

int Matrix::gCol() const
{
	return col;
}

Matrix::Matrix()
{
	vv = vector<double>();
}

Matrix::Matrix(int row, int col)
{
	this->row = row;
	this->col = col;
	vv = vector<double>(row*col);
}

Matrix::Matrix(int row, int col, double val) : Matrix(row, col)
{
	for (int i = 0; i < row*col; i++)
		vv[i] = val;
}

Matrix::Matrix(int r, int c, double * p) : row(r), col(c)
{
	vv = vector<double>(p, p + row*col);
}

double * Matrix::getpA()
{
	return &vv[0];
}

double* Matrix::operator[](int i)
{
	return &vv[i*col];
}

Matrix Matrix::operator*(Matrix &m)
{
	if (gCol() != m.gRow()) return Matrix();
	Matrix newMatr(gRow(), m.gCol());
	for (int z = 0; z < gRow(); z++)
		for (int i = 0; i < m.gCol(); i++)
			for (int j = 0; j < m.gRow(); j++)
				newMatr[z][i] += (*this)[z][j] * m[j][i];
	return newMatr;
}

Matrix Matrix::operator*(double val)
{
	Matrix m(gRow(), gCol());
	for (int i = 0; i < gRow(); i++)
		for (int j = 0; j < gCol(); j++)
			m[i][j] = (*this)[i][j] * val;
	return m;
}

Matrix Matrix::operator/(double val)
{
	Matrix m(gRow(), gCol());
	for (int i = 0; i < gRow(); i++)
		for (int j = 0; j < gCol(); j++)
			m[i][j] = (*this)[i][j] / val;
	return m;
}

Matrix Matrix::createDiagDominMatrix(int n, int min, int max)
{
	Matrix m(n, n);
	srand(time(0));
	for (int i = 0; i < n; i++)
	{
		int sum = 0;
		for (int j = 0; j < n; j++)
			if (j != i)
			{
				int el = rand() % (max - min + 1) + min;
				m[i][j] = el;
				sum += abs(el);
			}
		m[i][i] = sum + rand() % (max - min + 1);
	}
	return m;
}

std::ostream & operator<<(std::ostream & os, Matrix & m)
{
	if (m.gRow() == 0) os << 0;
	for (int i = 0; i < m.gRow(); i++)
	{
		for (int j = 0; j < m.gCol(); j++)
			os << m[i][j] << ' ';
		os << '\n';
	}
	os << '\n';
	return os;
}