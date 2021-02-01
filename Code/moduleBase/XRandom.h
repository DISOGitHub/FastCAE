#pragma once
class XRandom
{
public:
	XRandom();
	~XRandom();

	///<MG 高斯分布 mean 期望  stdc 标准差
	double GaussRand(const double & mean , const double & stdc);

	///<MG 泊松分布
	int Possion(const double & lambda);

	///<MG 指数分布 
	double Exponential(const double & lambda);

	///<MG 生成随机数
	double Rand();

	///<MG 生成范围随机数
	double Rand(const double & from , const double & to);

	///<MG 单例
	static XRandom * instance();

private:
	double GaussRand_NORMAL();

	double U_Random();

	static XRandom * _instance;
};

