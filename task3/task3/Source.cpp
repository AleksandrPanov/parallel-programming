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
}
void FillData(int n, vec &points, double start, double end, func f1, funcPeriodic f2, int rank)
{
	points = vec(n);
	if (rank == 0)
	{
		double delta = (end - start) / n, x = start;
		for (int i = 0; i < n; i++)
		{
			if (f1 != 0)
				points[i] = f1(x);
			else if (f2 != 0)
				points[i] = f2(x, end);
			x += delta;
		}
	}
}
void PostAllData(int n, vec &points, int rank)
{
	MPI_Bcast(points.getP(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}
int main(int argc, char **argv)
{
	int rank, size, n = 101;
	vec points;
	if (argc > 1)
	{
		n = atoi(argv[1]);
	}
	MPI_Start(argc, argv, size, rank);
	cout << "The number of processes: " << size << " my number is " << rank << '\n';
	FillData(n, points, 0, 2*pi, sin, 0, rank);
	PostAllData(n, points, rank);
	MPI_Finalize();
	return 0;
}