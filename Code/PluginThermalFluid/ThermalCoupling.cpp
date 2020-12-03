#include "ThermalCoupling.h"
#include <QDomDocument>
#include <qDebug>

namespace ThermalFluid{
	ThermalCoupling::ThermalCoupling()
	{
	}


	ThermalCoupling::~ThermalCoupling()
	{
	}

	void ThermalCoupling::setGroupOne(QString g)
	{
		_groupOne = g;
	}

	QString ThermalCoupling::getGroupOne()
	{
		return _groupOne;
	}

	void ThermalCoupling::setGroupTwo(QString g)
	{
		_groupTwo = g;
	}

	QString ThermalCoupling::getGroupTwo()
	{
		return _groupTwo;
	}

	void ThermalCoupling::setType(CouplingType t)
	{
		_type = t;
	}

	CouplingType ThermalCoupling::getType()
	{
		return _type;
	}

	void ThermalCoupling::setXValue(double v)
	{
		_xValue = v;
	}

	double ThermalCoupling::getXValue()
	{
		return _xValue;
	}

	void ThermalCoupling::setCoe(double coe)
	{
		_coe = coe;
	}

	void ThermalCoupling::setCoe(double xl, double xf, double xt)
	{
		if (_type == NEAR)
			_coe = _xValue * xt / xl / xf;
		else if ((_type == NEARP) || (_type == NEARLP))
			_coe = _xValue * xt / xf;
		else if ((_type == NEARA) || (_type == CONV))
			_coe = _xValue * xt * xl / xf;
		else if (_type == NEARM)
			_coe = 0;
		else if ((_type == NEARAR) || (_type == RAD) || (_type == NEARAR2))
			_coe = _xValue;
		else if (_type == NEARRES)
			_coe = _xValue * xf * xl / xt;
		appendProperty(QObject::tr("coefficient of heat transfer"), _coe);
	}

	double ThermalCoupling::getCoe()
	{
		return _coe;
	}

	CouplingType ThermalCoupling::stringToCouplingType(QString s)
	{
		s = s.toUpper();
		if (s == "NEAR")
			return NEAR;
		else if (s == "NEARP")
			return NEARP; 
		else if (s == "NEARA")
			return NEARA;
		else if (s == "NEARM")
			return NEARM;
		else if (s == "NEARLP")
			return NEARLP;
		else if (s == "NEARAR")
			return NEARAR;
		else if (s == "RAD")
			return RAD;
		else if (s == "NEARAR2")
			return NEARAR2;
		else if (s == "NEARRES")
			return NEARRES;
		else if (s == "CONV")
			return CONV;
				
	}

	QString ThermalCoupling::couplingTypeToString(CouplingType t)
	{
		QString s;
		if (t == NEAR)
			s = "NEAR";
		else if (t == NEARP)
			s = "NEARP";
		else if (t == NEARA)
			s = "NEARA";
		else if (t == NEARM)
			s = "NEARM";
		else if (t == NEARLP)
			s = "NEARLP";
		else if (t == NEARAR)
			s = "NEARAR";
		else if (t == RAD)
			s = "RAD";
		else if (t == NEARAR2)
			s = "NEARAR2";
		else if (t == NEARRES)
			s = "NEARRES";
		else if (t == CONV)
			s = "CONV";
		return s;
	}

	void ThermalCoupling::writeTcData(QDomDocument* doc, QDomElement* tc)
	{
		QDomElement aThermalCoupling = doc->createElement("aThermalCoupling");
		aThermalCoupling.setAttribute("id",getID());
		aThermalCoupling.setAttribute("name", getName());
		aThermalCoupling.setAttribute("GroupOne", _groupOne);
		aThermalCoupling.setAttribute("GroupTwo", _groupTwo);
		aThermalCoupling.setAttribute("Type", couplingTypeToString(_type));
		aThermalCoupling.setAttribute("XValue", QString::number(_xValue, 'f', 10));
		aThermalCoupling.setAttribute("Coe", QString::number(_coe, 'f', 10));
		tc->appendChild(aThermalCoupling);
	}

	void ThermalCoupling::readTcData(QDomElement* aTc)
	{
		if (!aTc->isNull())
		{
			setID(aTc->attribute("id").toInt());
			setName(aTc->attribute("name"));

			_groupOne = aTc->attribute("GroupOne");
			_groupTwo = aTc->attribute("GroupTwo");
			_xValue   = aTc->attribute("XValue").toDouble();
			_coe      = aTc->attribute("Coe").toDouble();
			_type     = stringToCouplingType(aTc->attribute("Type"));
		}
	}

}