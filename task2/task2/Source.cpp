#include<mpi.h>
#include<iostream>
#include "Matrix.h"
#include <cassert>
#include <algorithm>
#include <stddef.h>
#define tag 0
#define col1 row
using namespace std;
char *read = "in.txt", *write = "out.txt";
#define sizeRowInSendedRow 50
typedef struct {
	int numRow;
	int curRow;
	double row[sizeRowInSendedRow];
} SendedRow;
void prepareStruct(MPI_Datatype &sendedRow)
{
	int len[4] = { 1, 1, sizeRowInSendedRow, 1};
	MPI_Aint pos[4] = { offsetof(SendedRow,numRow), offsetof(SendedRow, curRow),	offsetof(SendedRow, row),sizeof(SendedRow) };
	MPI_Datatype typ[4] = { MPI_INT, MPI_INT, MPI_DOUBLE, MPI_UB};
	MPI_Type_struct(4, len, pos, typ, &sendedRow);
	MPI_Type_commit(&sendedRow);
}
void clearStruct(MPI_Datatype &sendedRow)
{
	MPI_Type_free(&sendedRow);
}
void createSendedStruct(int numRows, int sizeRow, SendedRow &sendedRow, double* dataA, double* dataF, int curRow)
{
	sendedRow.numRow = numRows;
	sendedRow.curRow = curRow;
	for (int i = 0; i < numRows*sizeRow; i++)
		sendedRow.row[i] = dataA[i + curRow*sizeRow];

	for (int i = 0; i < numRows; i++)
		sendedRow.row[i + numRows*sizeRow] = dataF[i+curRow];
}
void createSendedStructX(int numRows, SendedRow &sendedRow, double* dataX, int curRow)
{
	sendedRow.numRow = numRows;
	sendedRow.curRow = curRow;
	for (int i = 0; i < numRows; i++)
		sendedRow.row[i+curRow] = dataX[i+curRow];
}
void createSendedResX(int numRows, SendedRow &sendedRow, double* dataX)
{
	for (int i = 0; i < numRows; i++)
		sendedRow.row[i] = dataX[i];
}
Matrix getF(Matrix &m, Matrix &x0, Matrix &f, int start)			  //(0 x2) * -xx1 + f1 
{																	  //(0  0) * -xx2 + f2

	int row = m.gRow();
	int col = m.gCol();
	Matrix newF(row, 1);
	for (int i = 0; i < row; i++)
	{
		newF[i][0] += f[i][0];
		for (int j = start+i+1; j < col; j++)
			newF[i][0] -= m[i][j]* x0[j][0];
	
	}
	return newF;
}
void unPackAF(SendedRow &mySendedRow, MPI_Status &status, int &row, int &numRow, int &curRow, Matrix &A, Matrix &F)
{
	row = status.MPI_TAG;
	numRow = mySendedRow.numRow;
	curRow = mySendedRow.curRow;
	double *pA = mySendedRow.row;
	double *pF = mySendedRow.row + numRow*col1;
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
			f[i][0] -= m[i][j+startX] * tx[j+startX][0];
}
void setX(Matrix &m, Matrix &f, Matrix &x, Matrix &x0, int curRow, int nRow)
{
	for (int i = 0; i < nRow; i++)
	{
		for (int j = curRow; j < i + curRow; j++)
			f[i][0] -= m[i][j] * x[j][0];
		x[i+curRow][0] = f[i][0] / m[i][i + curRow];
		x0[i+curRow][0] = x[i+curRow][0];
	}
}
double* readSystem(int &len)
{
	freopen(read, "r", stdin);
	scanf("%d", &len);
	double *p = new double[len*len + len];
	for (int i = 0; i < len*len + len; i++)
		scanf("%lf", p+i);
	return p;
}
int getSizeTask1(int i, int nRow, int nProc)
{
	if (i == 0) return (nRow / nProc) + ( nRow % nProc);
	return nRow / nProc;
}
void setTX(Matrix &tX, Matrix &x, int tCurRow, int tNumRow)
{
	for (int i = tCurRow; i < tCurRow + tNumRow; i++)
		x[i][0] = tX[i][0];
}
int main(int argc, char **argv)
{
	Matrix startA, tmpF;
	Matrix A, F, x0, x1;
	int rank, nProc, row , curRow, numRow, nRep = 200000;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Request request[1];
	MPI_Status status;
	MPI_Datatype sendedRow;
	SendedRow mySendedRow;
	prepareStruct(sendedRow);
	if (rank == 0)
	{
		//read file
		//cout << Matrix::createDiagDominMatrix(10, 0, 1);
		double *startData;
		startData = readSystem(row);
		startA = A = Matrix(row, row, startData);
		F = Matrix(row, 1, startData + row*row);
		curRow = 0;
		numRow = getSizeTask1(0, row, nProc);
		int tmpCurRow = 0, tmpNumRow = numRow;
		for (int i = 1; i < nProc; i++)
		{
			tmpCurRow += tmpNumRow;
			tmpNumRow = getSizeTask1(i, row, nProc);
			createSendedStruct(tmpNumRow, col1, mySendedRow, A.getpA(), F.getpA(), tmpCurRow);
			MPI_Send(&mySendedRow, 1, sendedRow, i, row, MPI_COMM_WORLD);
		}
		A = Matrix(numRow, col1, startData);	F = Matrix(numRow, 1, startData + row*col1);
		tmpF = F;
	}
	else
	{
		MPI_Recv(&mySendedRow, 1, sendedRow, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		unPackAF(mySendedRow, status, row, numRow, curRow, A, tmpF);
		F = tmpF;
	}
	x1 = Matrix(row, 1);
	x0 = x1;
	double startTime = MPI_Wtime();
	for (int z = 0; z < nRep; z++)
	{
		F = getF(A, x0, tmpF, curRow);
		Matrix tX; // taken x
		int tRow, tCurRow;
		for (int i = 0; i < rank; i++)
		{
			MPI_Recv(&mySendedRow, 1, sendedRow, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			unPackX(mySendedRow, tRow, tCurRow, row, tX);
			setTX(tX, x1, tCurRow, tRow);
			setFByX(A, tX, F, tCurRow, tRow);
		}
		setX(A, F, x1, x0, curRow, numRow);
		createSendedStructX(numRow, mySendedRow, x1.getpA(), curRow);

		for (int i = rank + 1; i < nProc; i++)
		{
			MPI_Isend(&mySendedRow, 1, sendedRow, i, tag, MPI_COMM_WORLD, request);
		}
		//отправляем результат
		if (rank == nProc - 1)
		{
			createSendedResX(row, mySendedRow, x1.getpA());
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