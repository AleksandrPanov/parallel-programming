#pragma once
class vec
{
private:
	double *p = 0;
	int n = 0;
public:
	int size() { return n; }
	vec(){}
	vec(int size) : n(size)
	{
		p = new double[n];
	}
	vec(const vec &v): n(v.n)
	{
		delete[] p;
		p = new double[n];
		for (int i = 0; i < n; i++)
			p[i] = v.p[i];
	}

	vec& operator = (const vec &v)
	{
		if (this != &v)
		{
			if (p != 0)
				delete[] p;
			n = v.n;
			p = new double[n];
			for (int i = 0; i < n; i++)
				p[i] = v.p[i];
		}
		return *this;
	}
	double* getP() { return p; }
	double& operator[](int i) const
	{
		return p[i];
	}
	void setP(int n, double *p)//не очищает память
	{
		this->n = n;
		this->p = p;
	}
	~vec()
	{
		if (p != 0)
		{
			delete[] p;
			p = 0;
		}
	}
};