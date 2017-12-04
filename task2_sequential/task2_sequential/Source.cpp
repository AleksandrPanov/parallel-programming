#include "Matrix.h"
#include "GaussMethod.h"
#include <cassert>
#include <iostream>
using namespace std;

Matrix getF(Matrix &m, Matrix &x, Matrix &f) //(0 x2) * -xx1 + f1 
{											 //(0  0) * -xx2 + f2
	assert(x.gCol() == 1 && f.gCol() == 1 && m.gRow() == x.gRow() && m.gRow() == f.gRow());
	int row = m.gRow();
	int col = m.gCol();
	Matrix newF(row, 1);
	for (int i = 0; i < row; i++)
	{
		newF[i][0] += f[i][0];
		for (int j = i+1; j < col; j++)
			newF[i][0] -= m[i][j] * x[j][0];
	}
	return newF;
}		

int main()
{
	Matrix A, f, x1, f1;
	cin >> A >> f;
	int m = 20;
	Matrix x0(f.gRow(), 1);
	for (int i = 0; i < m; i++)
	{
		f1 = getF(A, x0, f);
		x1 = GaussMethod::getSolve(A, f1);
		x0 = x1;
	}
	cout << x1;
	return 0;
}