#include<mpi.h>
#include<iostream>
#include <cmath>
#include "FourierFunction.h"
#define tag 1
using std::cout;
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
			if (f1 != 0)
				points[i] = f1(x);
			else if (f2 != 0)
				points[i] = f2(x, r);
			x += delta;
		}
	}
}
void PostAllData(int n, vec &points, double lr[], int rank)
{
	MPI_Bcast(points.getP(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(lr, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}
int main(int argc, char **argv)
{
	int rank, size, n = 20001, m;
	vec points;
	FourierFunction fourierFunction;
	double lr[2] = { 0, 2 * pi }, startTime, endTime;
	if (argc > 1)
	{
		n = atoi(argv[1]);
	}
	m = n / 2;
	MPI_Start(argc, argv, size, rank);
	FillData(n, points, lr[0], lr[1], sin, 0, rank);
	PostAllData(n, points, lr, rank);
	if (rank == 0)
		startTime = MPI_Wtime();
	fourierFunction.setCoeff(n, fourierFunction.getM(rank, size, m), fourierFunction.getStartM(rank, size, m), lr[0], lr[1], points.getP());
	if (rank == 0)
	{
		endTime = MPI_Wtime() - startTime;
		cout << "time =  " <<endTime;
	}
	MPI_Finalize();
	return 0;
}