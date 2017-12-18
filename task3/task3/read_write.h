#pragma once
#pragma once
#include <fstream>
#include <string>
using std::string;
void writePoints(int n, double *ar, std::ofstream &of)
{
	for (int i = 0; i < n; i++)
		of << ar[i] << " ;";
}
void openForWrite(std::ofstream &f, string name, bool createNewFile)
{
	if (createNewFile)
		f.open(name + ".csv");
	else f.open(name + ".csv", std::ios::app);
}