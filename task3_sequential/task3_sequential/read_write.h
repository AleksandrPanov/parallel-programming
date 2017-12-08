#pragma once
#include <fstream>
using namespace std;
void writePoint(int n, double *ar, fstream &fs)
{
	for (int i = 0; i < n; i++)
		fs << ar[i] << ';';
}