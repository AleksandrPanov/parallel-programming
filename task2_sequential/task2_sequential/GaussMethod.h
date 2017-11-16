#pragma once
#include "Matrix.h"
#include <iostream>
class GaussMethod
{
protected:
	Matrix A;
	Matrix f;
	Matrix x;
	GaussMethod() {};
	GaussMethod(const Matrix &aa, const Matrix &ff, const Matrix &xx, int max) :A(aa), f(ff) {};
public:
	Matrix static getSolve(Matrix &A, Matrix &f); // * 0
												  // * *		
	Matrix solve() 
	{
		x = getSolve(A, f);
		return x;
	};
	friend std::istream & operator >> (std::istream &is, GaussMethod & m)
	{
		std::cout << "print A\n";
		is >> m.A;
		std::cout << "print f\n";
		is >> m.f;
		return is;
	}
};