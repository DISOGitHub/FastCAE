#include "XRandom.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>  

XRandom * XRandom::_instance = NULL;

XRandom::XRandom()
{
	srand((unsigned)time(NULL));
}

XRandom::~XRandom()
{

}

double XRandom::GaussRand(const double & mean, const double & stdc)
{
	return mean + GaussRand_NORMAL() * stdc;
}

///<MG lambda总体平均次数
int XRandom::Possion(const double & lambda)
{
	long double Lambda = lambda, k = 0;
	long double p = 1.0;
	long double l = exp(-Lambda);  /* 为了精度，才定义为long double的，exp(-Lambda)是接近0的小数*/
	while (p >= l)
	{
		double u = U_Random();
		p *= u;
		k++;
	}
	return k - 1;
}

double XRandom::Exponential(const double & lambda)
{
	double pV = 0.0;
	while (true)
	{
		pV = (double)rand() / (double)RAND_MAX;
		if (pV != 1)
		{
			break;
		}
	}
	pV = (-1.0 / lambda)*log(1 - pV);
	return pV;
}

double XRandom::Rand()
{
	double f;
	f = (double)rand();
	return f;
}

double XRandom::Rand(const double & from, const double & to)
{
	double f;
	int int_value_range = 0;
	double double_value_range = to - from;
	if (double_value_range > 1000)
	{
		int_value_range = (int)double_value_range;
		f = rand() % int_value_range;
		f += from;
	}
	else
	{
		double multiple = 1000 / double_value_range;
		f = rand() % 1000;
		f /= multiple;
		f += from;
	}
	return f;
}

XRandom * XRandom::instance()
{
	if (!_instance)
	{
		_instance = new XRandom();
	}
	return _instance;

}

double XRandom::GaussRand_NORMAL()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if (phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while (S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	}
	else
		X = V2 * sqrt(-2 * log(S) / S);

	phase = 1 - phase;
	return X;
}

double XRandom::U_Random()
{
	double f;
	f = (float)(rand() % 100);
	return f / 100;
}
