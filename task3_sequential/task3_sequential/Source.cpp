#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include "read_write.h"
using namespace std;
const double pi = 3.14159265358979323846;
using func = double(double);
func *f = sin;
func *g = [](double x)
{ 
	if (x < pi)
	return x;
	else return 2 * pi - x;
};
vector<double> setPointFunct(int n, double l, double r, func f)
{
	assert(n > 1);
	vector<double>ar(n);
	double delta = (r - l) / n;
	double x = l;
	for (int i = 0; i < n; i++)
	{
		ar[i] = f(x);
		x += delta;
	}
	return move(ar);
}
typedef vector<double> vec;
class FourierFunction
{
	vec A;
	vec B;
	int n;
	int m;
	int endM;
	double l;
	double r;
	double normCoeff =1.0;
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
	void setCoeff(int n, int m, int startM,  double l, double r, const double *y)
	{
		this->endM = endM;
		this->n = n;
		this->m = m;
		this->l = l;
		this->r = r;
		normCoeff = 2*pi/(r - l);
		normTerm = -l;
		A = vec(m+1);
		B = vec(m+1);
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
};
int main()
{
	double l = 0;
	double r = 2*pi;
	int n = 1001;
	vector<double> ar = setPointFunct(n, l, r, g);
	
	ofstream of;
	openForWrite(of, "sinX", true);
	writePoints(n, &ar[0], of);
	of.close();
	FourierFunction fourierFunction;
	int m = n/2;

	fourierFunction.setCoeff(n, m, 0,  l, r, &ar[0]);
	fourierFunction.setPoints(n, ar);
	openForWrite(of, "sinXFourier", true);
	writePoints(n, &ar[0], of);
	
	cout << fourierFunction.calculate(pi);

	//of.close();
	return 0;
}