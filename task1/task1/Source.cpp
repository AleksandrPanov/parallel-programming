#include <mpi.h>
#include<iostream>
using std::cin;
using std::cout;
#define tag 1
int main(int argc, char **argv)
{
	int rank, size, numPoint = 10;
	double ans = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int delta = (numPoint % size) - rank;
	if (delta > 0) 
		delta = 1;
	else delta = 0;
	
	double sum = 0;
	for (int i = 0; i < delta + numPoint / size; i++)
		sum += 1;
	//sum /= numPoint;

	cout << numPoint / size + delta<< ' ' <<sum<< " i count \n";
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
		cout << "ans = " << ans <<'\n';
	}
//	cout << "The number of processes: " << size << " my number is " << rank << '\n';
	MPI_Finalize();
	return 0;
}