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
public:
	FourierFunction(){}
	double getMemberSeries(int i, double x) const
	{
		return A[i] * cos(i*x) + B[i] * sin(i*x);
	}
	void setFunction(int n, int m, double *A, double *B)
	{
		delete[] A;
		delete[] B;
		this->n = n;
		this->m = m;
		this->A = A;
		this->B = B;
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

	return 0;
}