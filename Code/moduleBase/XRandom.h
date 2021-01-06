#pragma once
class XRandom
{
public:
	XRandom();
	~XRandom();

	///<MG ��˹�ֲ� mean ����  stdc ��׼��
	double GaussRand(const double & mean , const double & stdc);

	///<MG ���ɷֲ�
	int Possion(const double & lambda);

	///<MG ָ���ֲ� 
	double Exponential(const double & lambda);

	///<MG ���������
	double Rand();

	///<MG ���ɷ�Χ�����
	double Rand(const double & from , const double & to);

	///<MG ����
	static XRandom * instance();

private:
	double GaussRand_NORMAL();

	double U_Random();

	static XRandom * _instance;
};

