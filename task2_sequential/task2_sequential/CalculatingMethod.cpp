#include "calculatingMethod.h"

void CalculatingMethod::setA(const Matrix& a)
{
	A = a;
}

void CalculatingMethod::setF(const Matrix & f)
{
	this->f = f;
}

void CalculatingMethod::setX(const Matrix & x)
{
	this->x = x;
}

void CalculatingMethod::setStep(int step)
{
	this->step = step;
}

void CalculatingMethod::setAccuracy(double accuracy)
{
	this->accuracy = accuracy;
}

Matrix & CalculatingMethod::getA()
{
	return A;
}

Matrix & CalculatingMethod::getF()
{
	return f;
}

Matrix & CalculatingMethod::getX()
{
	return x;
}

int CalculatingMethod::getStep() const
{
	return step;
}

double CalculatingMethod::getAccuracy() const
{
	return accuracy;
}
