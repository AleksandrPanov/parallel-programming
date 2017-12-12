#pragma once
#include "vec.h"
double const pi = 3.14159265358979323846;
using func = double(double);
using funcPeriodic = double(double, double); //x, middle

class FourierFunction
{
	vec A;
	vec B;
	int n;
	int m;
	int mStart;
	int mEnd;
	double start;
	double end;
	double normCoeff = 1.0;
	double normTerm = 0;
	double gd(int n)
	{
		return 2 * pi / n;
	}
	double calc(double x) const
	{
		double res = A[0] / 2;
		for (int i = 1; i <= m; i++)
			res += getMemberSeries(i, x);
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
		return A[i] * cos(i*x) + B[i] * sin(i*x);
	}
	double getA0(const double *y)
	{
		double A0 = 0, x = 0, delta = gd(n);
		for (int j = 0; j < n; j++)
			A0 += y[j];
		A0 *= (2.0 / n);
		return A0;
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
	void setCoeff(int n, int m, double start, double end, const double *y)
	{
		this->n = n;
		this->m = m;
		this->start = start;
		this->end = end;
		normCoeff = 2 * pi / (end - start);
		normTerm = -start;
		A = vec(m + 1);
		B = vec(m + 1);
		for (int i = 0; i <= m; i++)
		{
			A[i] = getA(i, y);
			B[i] = getB(i, y);
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
};