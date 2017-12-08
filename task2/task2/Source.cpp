#include<mpi.h>
#include<iostream>
#include <algorithm>

#include "Matrix.h"
#include "sendedStruct.h"
#include "read_write.h"
using namespace std;
char *read = "in.txt", *tRead ="tin.txt" ;
Matrix getF(Matrix &m, Matrix &x0, Matrix &f, int start)			  //(0 x2) * -xx1 + f1 
{																	  //(0  0) * -xx2 + f2
	int row = m.gRow();
	int col = m.gCol();
	Matrix newF(row, 1);
	for (int i = 0; i < row; i++)
	{
		newF.vec(i) += f.vec(i);
		for (int j = start+i+1; j < col; j++)
			newF.vec(i) -= m[i][j]* x0.vec(j);
	
	}
	return newF;
}
void unPackAF(SendedMatrix &mySendedMatrix, MPI_Status &status, int &row, int &numRow, int &curRow, Matrix &A, Matrix &F)
{
	row = status.MPI_TAG;
	numRow = mySendedMatrix.numRow;
	curRow = mySendedMatrix.curRow;
	double *pA = mySendedMatrix.row;
	double *pF = mySendedMatrix.row + numRow*row;
	A = Matrix(numRow, row, pA);
	F = Matrix(numRow, 1, pF);
}
void unPackX(SendedRow &mySendedRow, int &numRow, int &curRow, int row, Matrix &X)
{
	numRow = mySendedRow.numRow;
	curRow = mySendedRow.curRow;
	double *pF = mySendedRow.row;
	X = Matrix(row, 1, pF);
}
void unPackResX(SendedRow &mySendedRow, int row, Matrix &X)
{
	double *pF = mySendedRow.row;
	X = Matrix(row, 1, pF);
}
void setFByX(Matrix &m, Matrix &tx, Matrix &f, int startX, int numX)
{
	for (int i = 0; i < m.gRow(); i++)
		for (int j = 0; j < numX; j++)
			f.vec(i) -= m[i][j+startX] * tx.vec(j + startX);
}
void setX(Matrix &m, Matrix &f, Matrix &x, Matrix &x0, int curRow, int nRow)
{
	for (int i = 0; i < nRow; i++)
	{
		for (int j = curRow; j < i + curRow; j++)
			f.vec(i) -= m[i][j] * x.vec(j);
		x.vec(i + curRow) = f[i][0] / m[i][i + curRow];
		x0.vec(i + curRow) = x.vec(i + curRow);
	}
}

int getSizeTask1(int i, int nRow, int nProc)
{
	if (i == 0) return (nRow / nProc) + ( nRow % nProc);
	return nRow / nProc;
}
void setTX(Matrix &tX, Matrix &x, int tCurRow, int tNumRow)
{
	for (int i = tCurRow; i < tCurRow + tNumRow; i++)
		x.vec(i) = tX.vec(i);
}

void readData(Matrix& A, Matrix& F, Matrix& x0, Matrix& x1, Matrix& startF,
	int &rank, int &nProc, int &row, int &curRow, int& numRow, char *r,
	MPI_Datatype& sendedMatrix, SendedMatrix& mySendedMatrix, MPI_Status &status)
{
	cout << "i work1\n";
	if (rank == 0)
	{
		//read file
		double *startData;
		startData = readSystem(r, row);
		Matrix startA = A = Matrix(row, row, startData);
		F = Matrix(row, 1, startData + row*row);
		curRow = 0;
		numRow = getSizeTask1(0, row, nProc);
		int tmpCurRow = 0, tmpNumRow = numRow;
		for (int i = 1; i < nProc; i++)
		{
			tmpCurRow += tmpNumRow;
			tmpNumRow = getSizeTask1(i, row, nProc);
			createSendedMatrix(tmpNumRow, row, mySendedMatrix, A.getpA(), F.getpA(), tmpCurRow);
			MPI_Send(&mySendedMatrix, 1, sendedMatrix, i, row, MPI_COMM_WORLD);
		}
		A = Matrix(numRow, row, startData);
		startF = Matrix(numRow, 1, startData + row*row);
	}
	else
	{
		MPI_Recv(&mySendedMatrix, 1, sendedMatrix, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		unPackAF(mySendedMatrix, status, row, numRow, curRow, A, startF);
	}
	x1 = Matrix(row, 1);
	x0 = x1;
}

int main(int argc, char **argv)
{
	Matrix A, F, x0, x1, startF;
	int rank, nProc, row , curRow, numRow, nRep = 100;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Request request[1];
	MPI_Status status;
	MPI_Datatype sendedRow, sendedMatrix;
	SendedRow mySendedRow;
	SendedMatrix mySendedMatrix;
	prepareStruct(sendedRow, sendedMatrix);
	writeSystem(tRead, 600, 0, 1);
	readData(A, F, x0, x1, startF, rank, nProc, row, curRow, numRow, tRead, sendedMatrix, mySendedMatrix, status);
	double startTime = MPI_Wtime();
	MPI_Type_free(&sendedMatrix);
	for (int z = 0; z < nRep; z++)
	{
		F = getF(A, x0, startF, curRow);
		int tRow, tCurRow;
		for (int i = 0; i < rank; i++)
		{
			Matrix tX; // taken x
			MPI_Recv(&mySendedRow, 1, sendedRow, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			unPackX(mySendedRow, tRow, tCurRow, row, tX);
			setTX(tX, x1, tCurRow, tRow);
			setFByX(A, tX, F, tCurRow, tRow);
		}
		setX(A, F, x1, x0, curRow, numRow);
		createSendedStructX(numRow, mySendedRow, x1.getpA(), curRow);

		for (int i = rank + 1; i < nProc; i++)
		{
			MPI_Isend(&mySendedRow, 1, sendedRow, i, 0, MPI_COMM_WORLD, request);
		}
		//отправляем результат
		if (rank == nProc - 1)
		{
			createSendedStructX(row, mySendedRow, x1.getpA(), 0);
		}
		MPI_Bcast(&mySendedRow, 1, sendedRow, nProc-1, MPI_COMM_WORLD);
		if (rank != nProc - 1)
		{
			unPackResX(mySendedRow, row, x0);
		}
	}
	if (rank == nProc - 1)
	{
		cout <<"answer:\n"<< x1<<"time = "<<MPI_Wtime() - startTime<<'\n';
	}
	clearStruct(sendedRow);
	MPI_Finalize();
	return 0;
}