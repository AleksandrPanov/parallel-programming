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
	int startM;
	double l;
	double r;
	double normCoeff = 1.0;
	double normTerm = 0;
	double gd(int n)
	{
		return 2 * pi / n;
	}
	double calc(double x) const
	{
		double res = (startM != 0 ? 0 : A[0] / 2);
		int start = (startM != 0 ? startM : 1);
		for (int i = start; i <= m; i++)
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
		A = vec(m + 1);
		B = vec(m + 1);
		for (int i = 0; i <= m; i++)
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
		return (allM / size) * rank;
	}
	int getM(int rank, int size, int allM)
	{
		return ( (rank != size - 1) ? (allM / size) : (allM / size + allM % size) );
	}
};