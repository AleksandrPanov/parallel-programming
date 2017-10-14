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
using CalculateFunc = double(double x);
CalculateFunc *funcAr[2] = {sin, cos};
int main(int argc, char **argv)
{
	int rank, size, numPoint = 1000000, numFunc = 0;
	double ans = 0, a = -10*pi, b = 10*pi;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	std::mt19937_64 gen(time(0));
	std::uniform_real_distribution<> genFromAtoB(a + rank*(b - a)/(double)size, a + (rank+1)*(b - a) / (double)size);
	//std::uniform_real_distribution<> genFromAtoB(a,b);


	int delta = getDelta(numPoint, size, rank);	

	double sum = 0;
	for (int i = 0; i < delta + numPoint / size; i++)
	{
		double x = genFromAtoB(gen);
		sum += funcAr[numFunc](x);
	}
	sum /= numPoint;

	//cout << numPoint / size + delta<< ' ' <<sum<< " count sum\n";
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
		cout << "ans = " << ans <<'\n';
	}
//	cout << "The number of processes: " << size << " my number is " << rank << '\n';
	MPI_Finalize();
	return 0;
}