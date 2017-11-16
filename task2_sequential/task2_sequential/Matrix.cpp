#include "Matrix.h"
#include <algorithm>
#include <iostream>
using namespace std;

int Matrix::gRow() const
{
	return vv.size();
}

int Matrix::gCol() const
{
	if (gRow() == 0)
		return 0;
	return vv[0].size();
}

Matrix::Matrix()
{
	vv = vector<vector<double>>();
}

Matrix::Matrix(int row, int col)
{
	vv = vector<vector<double>>(row);
	for (int i = 0; i < row; i++)
		vv[i] = vector<double>(col);
}

Matrix::Matrix(int row, int col, double val): Matrix(row, col)
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			vv[i][j] = val;
}

std::vector<double>& Matrix::operator[](int i)
{
	return vv[i];
}

Matrix Matrix::operator*(Matrix &m)
{
	if (gCol() != m.gRow()) return Matrix();
	Matrix newMatr(gRow(), m.gCol());
	for (int z = 0; z < gRow(); z++)
		for (int i = 0; i < m.gCol(); i++)
			for (int j = 0; j < m.gRow(); j++)
				newMatr[z][i] += vv[z][j] * m[j][i];
	return newMatr;
}

Matrix Matrix::operator*(double val)
{
	Matrix m(gRow(), gCol());
	for (int i = 0; i < gRow(); i++)
		for (int j = 0; j < gCol(); j++)
			m.vv[i][j] = vv[i][j] * val;
	return m;
}

Matrix Matrix::operator/(double val)
{
	Matrix m(gRow(), gCol());
	for (int i = 0; i < gRow(); i++)
		for (int j = 0; j < gCol(); j++)
			m.vv[i][j] = vv[i][j] / val;
	return m;
}

Matrix Matrix::operator+(Matrix & m)
{
	Matrix ans(gRow(), gCol());
	for (int i = 0; i < gRow(); i++)
		for (int j = 0; j < gCol(); j++)
			ans.vv[i][j] = vv[i][j] + m.vv[i][j];
	return ans;
}

Matrix Matrix::operator-(Matrix & m)
{
	Matrix ans(gRow(), gCol());
	for (int i = 0; i < gRow(); i++)
		for (int j = 0; j < gCol(); j++)
			ans.vv[i][j] = vv[i][j] - m.vv[i][j];
	return ans;
}

double Matrix::getNorm()
{
	double ans = 0.0;
	for (int i = 0; i < gRow(); i++)
	{
		double tmp = 0.0;
		for (int j = 0; j < gCol(); j++)
			tmp += abs(vv[i][j]);
		ans = max(tmp, ans);
	}
	return ans;
}

void Matrix::substractRow(int i, int j, double val)
{
	for (int z = 0; z < gCol(); z++)
		vv[i][z] -= vv[j][z] * val;
}

void Matrix::swapRow()
{
	Matrix m(*this);
	for (int i = 0; i < gRow(); i++)
		vv[i] = m[gRow() - 1 - i];
}

Matrix Matrix::createE(int i)
{
	Matrix E(i, i);
	for (int i = 0; i < E.gRow(); i++)
		E[i][i] = 1.0;
	return E;
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

std::istream & operator >> (std::istream & is, Matrix & m)
{
	int row, col;
	cout << "print row and col\n";
	is >> row >> col;
	m = Matrix(row, col);
	for (int i = 0; i < m.gRow(); i++)
		for (int j = 0; j < m.gCol(); j++)
			is >> m[i][j];
	return is;
}
