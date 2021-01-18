#pragma once
#include "QString"
#include "QSize"
#include "moduleBaseAPI.h"

class RandomPrivate;

enum RandomMode
{
	Mode_Single_Value = 1,
	Mode_Normal = 2,
	Mode_Linearity = 4,
	Mode_Gauss = 8,
	Mode_Possion = 16,
	Mode_Expontial = 32,
	Mode_ALL = Mode_Single_Value | Mode_Normal | Mode_Linearity | Mode_Gauss | Mode_Possion | Mode_Expontial,
	Mode_Multi_Value = Mode_Normal | Mode_Linearity | Mode_Gauss | Mode_Possion | Mode_Expontial,
};

class MODULEBASEAPI Random
{
	//Q_OBJECT
public:
	Random();
	~Random();

	int exec(const QString & name, int min, int max, int mode = Mode_ALL, const QSize size = QSize());

	QList<double> getRandomValues() const;
private:
	RandomPrivate * m_private;
};

