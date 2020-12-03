#include "Position.h"
#include "Value3D.h"
#include <QDomElement>

namespace Geant4
{
	Geant4::Position::Position()
	{
		_value = new Value3D;
		_dim = 3;
		_type = DataType::PositionType;
	}

	Position::~Position()
	{
		delete _value;
	}

	void Position::readGDML(QDomElement & ele)
	{
		QString name = ele.attribute("name");
		this->setName(name);
		QString unit = ele.attribute("Unit");
		this->setUnit(unit);
		
		QString x = ele.attribute("x");
		QString y = ele.attribute("y");
		QString z = ele.attribute("z");
		_value->setValue(x, y, z);
		if (x.isEmpty())
			this->appendProperty("X", 0);
		else
			this->appendProperty("X", x);
		if (y.isEmpty())
			this->appendProperty("Y", 0);
		else
			this->appendProperty("Y", y);
		if (z.isEmpty())
			this->appendProperty("Z", 0);
		else
			this->appendProperty("Z", z);
	}

	void Position::writeGDML(QDomDocument& doc, QDomElement& ele)
	{
		QDomElement pos = doc.createElement("position");
		pos.setAttribute("name", this->getName());
		pos.setAttribute("unit", this->getUnit());
		QStringList list;
		this->getValue(list);
		pos.setAttribute("x", list.at(0));
		pos.setAttribute("y", list.at(1));
		pos.setAttribute("z", list.at(2));
		ele.appendChild(pos);
	}

	void Position::setUnit(QString u)
	{
		if (u.isEmpty()) return;
		_value->setUnit(u);
		this->appendProperty("Unit", u);
	}

	QString Position::getUnit()
	{
		return _value->getUnit();
	}

	void Position::setValue(double* v)
	{
		_value->setValue(v[0], v[1], v[2]);
	}

	void Position::setValue(QString x, QString y, QString z)
	{
		_value->setValue(x, y, z);
	}

	void Position::getValue(double* v)
	{
		return _value->getValue(v);
	}

	void Position::getValue(QStringList& list)
	{
		_value->getValue(list);
	}

}



