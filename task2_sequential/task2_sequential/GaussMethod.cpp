#include "GaussMethod.h"

Matrix GaussMethod::getSolve(Matrix & A, Matrix & f)
{
	Matrix ans(A.gRow(), 1);
	for (int i = 0; i < A.gRow();i++)
	{
		double tmp = f[i][0];
		for (int j = 0; j < i; j++)
		{
			tmp -= A[i][j] * ans[j][0];
		}
		ans[i][0] = tmp / A[i][i];
	}
	return ans;
}
