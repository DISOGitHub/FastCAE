#include "TFThickness.h"
#include <QDomDocument>

namespace ThermalFluid{
	TFThickness::TFThickness()
	{
	}


	TFThickness::~TFThickness()
	{
	}

// 	void TFThickness::setId(int id)
// 	{
// 		_id = id;
// 	}
// 
// 	int TFThickness::getId()
// 	{
// 		return _id;
// 	}
// 
// 	void TFThickness::setName(QString name)
// 	{
// 		_name = name;
// 	}
// 
// 	QString TFThickness::getName()
// 	{
// 		return _name;
// 	}

	void TFThickness::setValue(double v)
	{
		_value = v;
	}

	double TFThickness::getValue()
	{
		return _value;
	}

	void TFThickness::wirteThickData(QDomDocument* doc, QDomElement* thick)
	{
		QDomElement aThick = doc->createElement("aThick");
		aThick.setAttribute("id",getID());
		aThick.setAttribute("name", getName());
		aThick.setAttribute("value", QString::number(_value, 'f', 10));
		thick->appendChild(aThick);						
	}

	void TFThickness::readThickData(QDomElement* aThick)
	{
		if (!aThick->isNull())
		{
			setID(aThick->attribute("id").toInt());
			setName(aThick->attribute("name"));
			
			_value = aThick->attribute("value").toDouble();
		}
	}

}