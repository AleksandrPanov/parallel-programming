#include <iostream>
#include <cassert>
#include <cmath>
using namespace std;
const double pi = 3.14159265359;
using func = double(double);
void setPointFunct(int n, double l, double r, double *ar, func f)
{
	assert(n > 1);
	ar = new double[n];
	double delta = (r - l) / n;
	double start = l;
	for (int i = 0; i < n; i++)
	{
		ar[i] = f(start);
		start += delta;
	}
}
void setCoefFourier(int n, int m, double *A, double *B)
{

}
class FourierFunction
{
	double *A = nullptr;
	double *B = nullptr;
	int n;
	int m;
	double a;
	double b;
public:
	FourierFunction(){}
	void setFunction(int n, int m, double *A, double *B)
	{
		delete[] A;
		delete[] B;
		this->n = n;
		this->m = m;
		this->A = A;
		this->B = B;
	}
	double getMemberSeries(int i, double x) const
	{
		return A[i] * cos(i*x) + B[i] * sin(i*x);
	}
	double getA0(double *y)
	{
		double A0 = 0, x = a, delta = (b - a) / n;
		for (int j = 0; j < m; j++)
			A0 += y[j];
		A0 *= (2.0 / m);
		return A0;
	}
	double getA(int i, double *y)
	{
		double A = 0, x = a, delta = (b-a)/n;
		for (int j = 0; j < m; j++)
		{
			A += y[j] * cos(i*x);
			x += delta;
		}
		A *= (2.0/pi);
		return A;
	}
	double getB(int i, double *y)
	{
		double B = 0, x = a, delta = (b - a) / n;
		for (int j = 0; j < m; j++)
		{
			B += y[j] * sin(i*x);
			x += delta;
		}
		B *= (2.0 / pi);
		return B;
	}
	void setCoff(double *y)
	{
		A[0] = getA0(y);
		for (int i = 1; i < n; i++)
		{
			A[i] = getA(i, y);
			B[i] = getB(i, y);
		}
	}
	double calculate(double x) const
	{
		double res = A[0] / 2;
		for (int i = 1; i < n; i++)
			res += getMemberSeries(i, x);
		return res;
	}
	~FourierFunction()
	{
		delete[] A;
		delete[] B;
	}
};
//double getPoint(double x, )
int main()
{
	FourierFunction f;
	return 0;
}