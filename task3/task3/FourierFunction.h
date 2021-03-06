#pragma once
#include "vec.h"
double const pi = 3.14159265358979323846;
using func = double(double);
using funcPeriodic = double(func f, double, double, double); //func, x, middle, end
#include <iostream>
using namespace std;
class FourierFunction
{
	vec A;
	vec B;
	int n;
	int m;
	int startM;
	double l;
	double r;
	double normCoeff = 1.0;
	double normTerm = 0;
	double gd(int n)
	{
		return 2 * pi / n;
	}
	int AmoreB(int a, int b)
	{
		if (a > b) return 1;
		return 0;
	}
	double calc(double x) const
	{
		double res = 0;
		int start = 0;
		if (startM == 0)
		{
			res = A[0] / 2;
			start = 1;
		}
		for (int i = start; i < m; i++)
		{
			res += getMemberSeries(i, x);
		}
		return res;
	}
public:
	FourierFunction() {}
	void changeX(double &x) const
	{
		x += normTerm;
		x *= normCoeff;
	}
	double getMemberSeries(int i, double x) const
	{
		return A[i] * cos((i + startM)*x) + B[i] * sin((i+startM)*x);
	}
	double getA(int i, const double *y)
	{
		double A = 0;
		for (int j = 0; j < n; j++)
		{
			A += y[j] * cos(i *j * 2 * pi / n);
		}
		A *= (2.0 / n);
		return A;
	}
	double getB(int i, const double *y)
	{
		double B = 0;
		for (int j = 0; j < n; j++)
		{
			B += y[j] * sin(i * j * 2 * pi / n);
		}
		B *= (2.0 / n);
		return B;
	}
	void setCoeff(int n, int m, int startM, double l, double r, const double *y)
	{
		this->startM = startM;
		this->n = n;
		this->m = m;
		this->l = l;
		this->r = r;
		normCoeff = 2 * pi / (r - l);
		normTerm = -l;
		A = vec(m);
		B = vec(m);
		for (int i = 0; i < m; i++)
		{
			A[i] = getA(i + startM, y);
			B[i] = getB(i + startM, y);
		}
	}
	double calculate(double x) const
	{
		changeX(x);
		return calc(x);
	}
	void setPoints(int n, vec &v)
	{
		v = vec(n);
		double x = 0, delta = gd(n);
		for (int i = 0; i < n; i++)
		{
			v[i] = calc(x);
			x += delta;
		}
	}
	int getStartM(int rank, int size, int allM)
	{
		int m = getM(rank, size, allM);
		if (AmoreB(allM % size, rank))
		{
			return (rank * m + rank);
		}
		return (rank * m + rank);
	}
	int getM(int rank, int size, int allM)
	{
		return (allM/size + AmoreB(allM % size, rank));
	}
	int getM()
	{
		return m;
	}
	int getStartM()
	{
		return startM;
	}
};