#include "ControlTemperature.h"
#include <QDomDocument>
#include <QMetaEnum>
#include <qDebug>
//#pragma execution_character_set("utf-8")

namespace ThermalFluid{
	ControlTemperature::ControlTemperature(ThermalData* data) :_thermalData(data)
	{
		
	}


	ControlTemperature::~ControlTemperature()
	{
	}

	void ControlTemperature::setHGroup(QString g)
	{
		_hGroup = g;
	}

	QString ControlTemperature::getHGroup()
	{
		return _hGroup;
	}

	void ControlTemperature::setSGroup(QString g)
	{
		_sGroup = g;
	}

	QString ControlTemperature::getSGroup()
	{
		return _sGroup;
	}

	void ControlTemperature::setLTemperature(double t)
	{
		_lt = t;
	}

	double ControlTemperature::getLTemperature()
	{
		return _lt;
	}

	void ControlTemperature::setHTemperature(double t)
	{
		_ht = t;
	}

	double ControlTemperature::getHTemperature()
	{
		return _ht;
	}

// 	void ControlTemperature::setTypeAndUnit(QString f)
// 	{
// 		if (f == "AREA")
// 		{
// 			_type = "热流";
// 			_unit = "W/m^2";
// 		}
// 		else if (f == "ABSO")
// 		{
// 			_type = "每单元";
// 			_unit = "W";
// 		}
// 		else if (f == "TOTA")
// 		{
// 			_type = "总功率";
// 			_unit = "W";
// 		}
// 		else if (f == "VOLU")
// 		{
// 			_type = "热产";
// 			_unit = "W/m^3";
// 		}
// 		else
// 		{
// 			_type = "未识别";
// 			_unit = "-";
// 		}
// 	}

	void ControlTemperature::setTypeAndUnit(ThermalType t)
	{
		_type = t;

		if (t == ThermalType::AREA)		//_type = "热流";
			_unit = "W/m^2";
		else if (t == ThermalType::ABSO)	//_type = "每单元";
			_unit = "W";		
		else if (t == ThermalType::TOTA)	//_type = "总功率";
			_unit = "W";
		else if (t == ThermalType::VOLU)	//_type = "热产";
			_unit = "W/m^3";
		else					//_type = "未识别";
			_unit = "-";
		
	}

	ThermalType ControlTemperature::getThermalType()
	{
		return _type;
	}

	double ControlTemperature::getQ()
	{

		double yl = _thermalData->getYLength();
		double yf = _thermalData->getYFroce();

		double q = 0;
		if (_type == ThermalType::AREA)
			q = _xValue * yl / yf;
		else if (_type == ThermalType::ABSO)
			q = _xValue / yl / yf;
		else if (_type == ThermalType::TOTA)
			q = _xValue / yl / yf;
		else if (_type == ThermalType::VOLU)
			q = _xValue * yl * yl / yf;
		
		return q;
	}

// 	QString ControlTemperature::getType()
// 	{
// 		return _type;
// 	}
// 
// 	QString ControlTemperature::getUnit()
// 	{
// 		return _unit;
// 	}

	void ControlTemperature::setXValue(double v)
	{
		_xValue = v;
	}

	double ControlTemperature::getXValue()
	{
		return _xValue;
	}

// 	void ControlTemperature::setQ(QString f, double yl, double yf)
// 	{
// 		if (f == "AREA")
// 		{
// 			_q = _xValue * yl / yf;
// 		}
// 		else if (f == "ABSO")
// 		{
// 			_q = _xValue / yl / yf;
// 		}
// 		else if (f == "TOTA")
// 		{
// 			_q = _xValue / yl / yf;
// 		}
// 		else if (f == "VOLU")
// 		{
// 			_q = _xValue * yl * yl / yf;
// 		}
// 		else
// 		{
// 			_q = 0;
// 		}
// 	}
// 
// 	void ControlTemperature::setQ(ThermalType t, double yl, double yf)
// 	{
// 		if (t == AREA)
// 		{
// 			_q = _xValue * yl / yf;
// 		}
// 		else if (t == ABSO)
// 		{
// 			_q = _xValue / yl / yf;
// 		}
// 		else if (t == TOTA)
// 		{
// 			_q = _xValue / yl / yf;
// 		}
// 		else if (t == VOLU)
// 		{
// 			_q = _xValue * yl * yl / yf;
// 		}
// 		else
// 		{
// 			_q = 0;
// 		}
// 	}
// 
// 	double ControlTemperature::getQ()
// 	{
// 		return _q;
// 	}

	void ControlTemperature::writeConTemperData(QDomDocument* doc, QDomElement* temper)
	{
		QDomElement aTemper = doc->createElement("aTemper");
		aTemper.setAttribute("id", getID());
		aTemper.setAttribute("name", getName());
		aTemper.setAttribute("hGroup", _hGroup);
		aTemper.setAttribute("sGroup", _sGroup);
		aTemper.setAttribute("lt", QString::number(_lt, 'f', 10));
		aTemper.setAttribute("ht", QString::number(_ht, 'f', 10));
		aTemper.setAttribute("unit", _unit);
		aTemper.setAttribute("xValue", QString::number(_xValue, 'f', 10));
		aTemper.setAttribute("type", _thermalData->thermalTypeToString(_type));
		temper->appendChild(aTemper);
	}

	void ControlTemperature::readConTemperData(QDomElement* aConTemper)
	{
		if (!aConTemper->isNull())
		{
			setID(aConTemper->attribute("id").toInt());
			setName(aConTemper->attribute("name"));

			_hGroup = aConTemper->attribute("hGroup");
			_sGroup = aConTemper->attribute("sGroup");
			_lt     = aConTemper->attribute("lt").toDouble();
			_ht     = aConTemper->attribute("ht").toDouble();
			_unit   = aConTemper->attribute("unit");
			_xValue = aConTemper->attribute("xValue").toDouble();
			_type   = _thermalData->stringToThermalType(aConTemper->attribute("type"));
		}
	}
}