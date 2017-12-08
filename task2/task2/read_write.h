#pragma once
#include<iostream>
#include <fstream>
#include "Matrix.h"
double* readSystem(char *read, int &len)
{
	freopen(read, "r", stdin);
	scanf("%d", &len);
	double *p = new double[len*len + len];
	for (int i = 0; i < len*len + len; i++)
		scanf("%lf", p + i);
	return p;
}
void writeSystem(char *write, int n, int min, int max)
{
	std::ofstream of;
	of.open(write/*, std::ios::app*/);
	of << n<<'\n';
	of << Matrix::createDiagDominMatrix(n, min, max)<<"\n";
	Matrix::createVector(n, min, max).getOstreamVector(of);
	of.close();
}