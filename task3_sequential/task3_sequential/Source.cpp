#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include "read_write.h"
using namespace std;
const double pi = 3.14159265358979323846;
using func = double(double);
func *f = sin;
func *g = [](double x){return x; };
vector<double> setPointFunct(int n, double l, double r, func f)
{
	assert(n > 1);
	vector<double>ar(n);
	double delta = (r - l) / n;
	double start = l;
	for (int i = 0; i < n; i++)
	{
		ar[i] = f(start);
		start += delta;
	}
	return move(ar);
}

class FourierFunction
{
	vector<double> A;
	vector<double> B;
	int n;
	int m;
	double start;
	double end;
	double normCoeff =1.0;
	double normTerm = 0;
	double gd(int n)
	{
		return 2 * pi / n;
	}
public:
	FourierFunction(){}
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
		A *= (2.0/n);
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
	void setCoeff(int n, int m, const double *y)
	{
		this->n = n;
		this->m = m;
		A = vector<double>(m+1);
		B = vector<double>(m+1);
		for (int i = 0; i <= m; i++)
		{
			A[i] = getA(i, y);
			B[i] = getB(i, y);
		}
	}
	double calculate(double x) const
	{
		changeX(x);
		double res = A[0] / 2;
		for (int i = 1; i <= m; i++)
			res += getMemberSeries(i, x);
		return res;
	}
	void setPoints(int n, vector<double> &v, double start, double end)
	{
		v = vector<double>(n);
		this->start = start;
		this->end = end;
		double x = 0, delta = gd(n);
		for (int i = 0; i < n; i++)
		{
			v[i] = calculate(x);
			x += delta;
		}
	}
};
int main()
{
	int n = 100;
	vector<double> ar = setPointFunct(n, 0, 2 * pi, sin);
	
	ofstream of;
	openForWrite(of, "sinX", true);
	writePoints(n, &ar[0], of);
	of.close();
	FourierFunction fourierFunction;
	int m = n/2;

	fourierFunction.setCoeff(n, m, &ar[0]);
	fourierFunction.setPoints(n, ar, 0, 2 * pi);
	openForWrite(of, "sinXFourier", true);
	writePoints(n, &ar[0], of);
	
	cout << fourierFunction.calculate(1);

	//of.close();
	return 0;
}