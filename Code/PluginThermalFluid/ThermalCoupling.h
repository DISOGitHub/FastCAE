#ifndef THERMALCOUPLING_H
#define THERMALCOUPLING_H

#include <QString>
#include "DataProperty/DataBase.h"

enum CouplingType
{
	BLANK = 0,
	NEAR,
	NEARP,
	NEARA,
	NEARM,
	NEARLP,
	NEARAR,
	RAD,
	NEARAR2,
	NEARRES,
	CONV
};

namespace ThermalFluid{
	class ThermalCoupling : public DataProperty::DataBase
	{
	public:
		ThermalCoupling();
		~ThermalCoupling();

		void setGroupOne(QString g);
		QString getGroupOne();

		void setGroupTwo(QString g);
		QString getGroupTwo();

		void setType(CouplingType t);
		CouplingType getType();

		void setXValue(double v);
		double getXValue();

		//ÐìÎÄÇ¿2020/6/10Ìí¼Ó
		void setCoe(double coe);
		void setCoe(double xl, double xp, double xt);
		double getCoe();

		CouplingType stringToCouplingType(QString s);
		QString couplingTypeToString(CouplingType t);

		void writeTcData(QDomDocument* doc, QDomElement* tc);
		void readTcData(QDomElement* aTc);
	private:
		QString _groupOne;
		QString _groupTwo;
		CouplingType _type{ BLANK };
		double _xValue{ 0 };
		double _coe{ 0 };

	};
}
#endif

