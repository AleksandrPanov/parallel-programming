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
	double& vec(int i);
	double* operator [] (int i);
	Matrix operator * (Matrix& m);
	Matrix operator * (double val);
	Matrix operator / (double val);
	std::ofstream& getOstreamVector(std::ofstream& os);
	friend std::ostream& operator<<(std::ostream& os, Matrix& m);

	static Matrix createDiagDominMatrix(int n, int min, int max);
	static Matrix createVector(int n, int min, int max);
};