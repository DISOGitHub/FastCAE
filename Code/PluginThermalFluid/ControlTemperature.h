#ifndef CONTROLTEMPERATURE_H
#define CONTROLTEMPERATURE_H

#include <QString>
#include "DataProperty/DataBase.h"
#include "ThermalData.h"


namespace ThermalFluid{
	class ThermalData;

	class ControlTemperature : public DataProperty::DataBase
	{
	public:
		ControlTemperature(ThermalData* data);
		~ControlTemperature();

		void setHGroup(QString g);
		QString getHGroup();

		void setSGroup(QString g);
		QString getSGroup();

		void setLTemperature(double t);
		double getLTemperature();

		void setHTemperature(double t);
		double getHTemperature();

		//void setTypeAndUnit(QString f);
// 		QString getType();	
// 		QString getUnit();

		void setXValue(double v);
		double getXValue();

		//void setQ(QString f, double yl, double yf);
		//double getQ();

		void setTypeAndUnit(ThermalType t);
		ThermalType getThermalType();
		//void setQ(ThermalType t, double yl, double yf);

		double getQ();
		void writeConTemperData(QDomDocument* doc, QDomElement* temper);
		void readConTemperData(QDomElement* aConTemper);
	private:
		QString _hGroup;
		QString _sGroup;
		double _lt{ 0 };
		double _ht{ 0 };
		//QString _type;
		QString _unit;
		double _xValue{ 0 };
		//double _q{ 0 };

		ThermalType _type;

		ThermalData* _thermalData;
	};

}
#endif // !CONTROLTEMPERATURE_H

