#include <mpi.h>
#include<iostream>
#include <random>
#include <ctime>
#include <cmath>

#define tag 1
double const pi = 3.14159265358979323846;
using std::cin;
using std::cout;
inline int getDelta(int numPoint, int size, int rank)
{
	int delta = (numPoint % size) - rank;
	return delta > 0;
}
inline double fx(double x)
{
	return x;
}
using CalculateFunc = double(double x);
CalculateFunc *funcAr[3] = {sin, cos, fx};
int main(int argc, char **argv)
{
	int rank, size, numPoint = 100000000, numFunc = 0;
	double ans = 0, a = 0, b = pi, startTime;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	std::mt19937_64 gen(time(0));
	std::uniform_real_distribution<> genFromAtoB(a + rank*(b - a)/(double)size, a + (rank+1)*(b - a) / (double)size);
	char c;
	if (rank == 0)
	{
		cout << "Do you want set a, b, func and num of point? Y N\n";
		cin >> c;
		if (c == 'Y')
		{
			cin >> a >> b >> numFunc >> numPoint;
		}
	}

	MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&numFunc, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&numPoint, 1, MPI_INT, 0, MPI_COMM_WORLD);

	startTime = MPI_Wtime();
	int delta = getDelta(numPoint, size, rank);	

	double sum = 0;
	for (int i = 0; i < delta + numPoint / size; i++)
	{
		double x = genFromAtoB(gen);
		sum += funcAr[numFunc](x);
	}
	sum /= numPoint;

	if (rank != 0)
		MPI_Send(&sum, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
	else
	{
		ans += sum;
		double tmp;
		MPI_Status status;
		for (int i = 0; i < size - 1; i++)
		{
			MPI_Recv(&tmp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
			ans += tmp;
		}
		ans *= (b - a);
		cout << "time = "<< MPI_Wtime() - startTime <<" ans = " << ans <<'\n';
	}
	MPI_Finalize();
	return 0;
}