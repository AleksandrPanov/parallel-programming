#include<mpi.h>
#include<iostream>
#include <cmath>
#include "FourierFunction.h"
#define tag 1
using std::cout;
func *f = [](double x)
{
	return x;
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
//	cout << "The number of processes: " << size << " my number is " << rank << '\n';
}
void FillData(int n, vec &points, double l, double r, func f1, funcPeriodic f2, int rank)
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
	}
}
void PostAllData(int n, vec &points)
{
	MPI_Bcast(points.getP(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}
int main(int argc, char **argv)
{
	int rank, size, n = 1001, m;
	vec points;
	FourierFunction fourierFunction;
	double lr[2] = { 0, 2 * pi }, startTime, endTime, res = 0, x = 1, allRes = 0;
	if (argc > 1)
	{
		n = atoi(argv[1]);
	}
	m = 5;

	MPI_Start(argc, argv, size, rank);
	FillData(n, points, lr[0], lr[1], f, g, rank);
	PostAllData(n, points);
	if (rank == 0)
		startTime = MPI_Wtime();
	fourierFunction.setCoeff(n, fourierFunction.getM(rank, size, m), fourierFunction.getStartM(rank, size, m), lr[0], lr[1], points.getP());
	res = fourierFunction.calculate(x);
	cout << res;
	MPI_Reduce(&res, &allRes, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		endTime = MPI_Wtime() - startTime;
		//cout << "time =  " <<endTime << " res in point x = "<<x<<" is "<<allRes<<'\n';
	}
	MPI_Finalize();
	return 0;
}