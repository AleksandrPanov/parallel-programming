#include<iostream>
#include <random>
#include <chrono>
#include <cmath>

double const pi = 3.14159265358979323846;
using std::cin;
using std::cout;
inline double fx(double x)
{
	return x;
}
using CalculateFunc = double(double x);
CalculateFunc *funcAr[3] = { sin, cos, fx };
int main(int argc, char **argv)
{
	int numPoint = 100000000, numFunc = 0;
	double a = 0, b = 2 * pi;

	std::mt19937_64 gen(time(0));
	std::uniform_real_distribution<> genFromAtoB(a, b);
	
	double sum = 0, x;
	char c;
	cout << "Do you want set a, b, func and num of point? Y N\n";
	cin >> c;
	if (c == 'Y')
	{
		cin >> a >> b >> numFunc >> numPoint;
	}
	auto start_time = std::chrono::steady_clock::now();
	for (int i = 0; i < numPoint; i++)
	{
		x = genFromAtoB(gen);
		sum += funcAr[numFunc](x);
	}
	sum /= numPoint;
	sum *= (b - a);
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	cout << "time = " << (double)elapsed_ms.count() / 1000 << " ans = " << sum << '\n';
	return 0;
}