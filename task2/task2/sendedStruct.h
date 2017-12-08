#pragma once
#define sizeRowInSendedRow 600
#define sizeSendedMatrix 100000
typedef struct {
	int numRow;
	int curRow;
	double row[sizeRowInSendedRow];
} SendedRow;
typedef struct {
	int numRow;
	int curRow;
	double row[sizeSendedMatrix];
} SendedMatrix;
void prepareStruct(MPI_Datatype &sendedRow, MPI_Datatype &sendedMatrix)
{
	{
		int len[4] = { 1, 1, sizeRowInSendedRow, 1 };
		MPI_Aint pos[4] = { offsetof(SendedRow,numRow), offsetof(SendedRow, curRow),	offsetof(SendedRow, row),sizeof(SendedRow) };
		MPI_Datatype typ[4] = { MPI_INT, MPI_INT, MPI_DOUBLE, MPI_UB };
		MPI_Type_struct(4, len, pos, typ, &sendedRow);
		MPI_Type_commit(&sendedRow);
	}
	{
		int len[4] = { 1, 1, sizeSendedMatrix, 1 };
		MPI_Aint pos[4] = { offsetof(SendedMatrix, numRow), offsetof(SendedMatrix, curRow),	offsetof(SendedMatrix, row),sizeof(SendedMatrix) };
		MPI_Datatype typ[4] = { MPI_INT, MPI_INT, MPI_DOUBLE, MPI_UB };
		MPI_Type_struct(4, len, pos, typ, &sendedMatrix);
		MPI_Type_commit(&sendedMatrix);
	}
}

void clearStruct(MPI_Datatype &sendedRow)
{
	MPI_Type_free(&sendedRow);
}
void createSendedMatrix(int numRows, int sizeRow, SendedMatrix &sendedMatrix, double* dataA, double* dataF, int curRow)
{
	sendedMatrix.numRow = numRows;
	sendedMatrix.curRow = curRow;
	for (int i = 0; i < numRows*sizeRow; i++)
		sendedMatrix.row[i] = dataA[i + curRow*sizeRow];

	for (int i = 0; i < numRows; i++)
		sendedMatrix.row[i + numRows*sizeRow] = dataF[i + curRow];
}
void createSendedStructX(int numRows, SendedRow &sendedRow, double* dataX, int curRow)
{
	sendedRow.numRow = numRows;
	sendedRow.curRow = curRow;
	for (int i = 0; i < numRows; i++)
		sendedRow.row[i + curRow] = dataX[i + curRow];
}