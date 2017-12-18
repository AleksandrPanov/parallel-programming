#include<mpi.h>
#include<iostream>
#include <cmath>
#include "FourierFunction.h"
#include "read_write.h"

#define tag 1
using std::cout;
func *interference = [](double x)
{
	return 0.1*sin(50*x);
};
func *f = [](double x)
{
	return  sin(x) + interference(x);
};
funcPeriodic *g = [](func f, double x, double middle, double end)
{
	if (x <= middle)
		return f(x);
	else return f(end - x);
};
void MPI_Start(int &argc, char **argv, int &size, int &rank)
{
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
}
void FillData(int n, vec &points, double l, double r, func f1, funcPeriodic f2, int rank, int print)
{
	points = vec(n);
	if (rank == 0)
	{
		double delta = (r - l) / n, x = l;
		for (int i = 0; i < n; i++)
		{
			if (f1 != 0 && f2 == 0)
				points[i] = f1(x);
			else if (f2 != 0)
				points[i] = f2(f1, x, (r-l)/2, r);
			x += delta;
		}
		if (print)
		{
			ofstream of;
			openForWrite(of, "original", true);
			writePoints(n, points.getP(), of);
			of.close();
		}
	}
}
void PostAllData(int n, vec &points)
{
	MPI_Bcast(points.getP(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}
void calculate(FourierFunction &four, double x, double &res, double &allRes)
{
	res = four.calculate(x);
	MPI_Reduce(&res, &allRes, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
}
int main(int argc, char **argv)
{
	int rank, size, n = 20001, m, print = 0;
	vec points;
	FourierFunction fourierFunction;
	double lr[2] = { 0, 2 * pi }, startTime, endTime, res = 0, x = pi/2, allRes = 0;
	if (argc > 1)
	{
		n = atoi(argv[1]);
		if (argc > 2)
		{
			print = atoi(argv[2]);
		}
	}
	m = n / 2 + 1;

	MPI_Start(argc, argv, size, rank);
	FillData(n, points, lr[0], lr[1], f, 0, rank, print);
	PostAllData(n, points);
	if (rank == 0)
		startTime = MPI_Wtime();
	fourierFunction.setCoeff(n, fourierFunction.getM(rank, size, m), fourierFunction.getStartM(rank, size, m), lr[0], lr[1], points.getP());
	calculate(fourierFunction, x, res, allRes);
	if (rank == 0)
	{
		endTime = MPI_Wtime() - startTime;
		cout << "time =  " <<endTime << " res in point x = "<<x<<" is "<<allRes<<'\n';
		if (print)
		{
			ofstream of;
			fourierFunction.setPoints(n, points);
			openForWrite(of, "out", true);
			writePoints(n, points.getP(), of);
		}
		
	}
	MPI_Finalize();
	return 0;
}