#include "Rotation.h"
#include "Value3D.h"
#include <QDomElement>

namespace Geant4
{
	Geant4::Rotation::Rotation()
	{
		_value = new Value3D;
		_dim = 3;
		_type = DataType::RotationType;
	}

	Rotation::~Rotation()
	{

	}

	void Rotation::readGDML(QDomElement & ele)
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

	void Rotation::writeGDML(QDomDocument& doc, QDomElement& ele)
	{
		QDomElement rot = doc.createElement("rotation");
		rot.setAttribute("name", this->getName());
		rot.setAttribute("unit", this->getUnit());
		QStringList list;
		this->getValue(list);
		rot.setAttribute("x", list.at(0));
		rot.setAttribute("y", list.at(1));
		rot.setAttribute("z", list.at(2));
		ele.appendChild(rot);
	}

	void Rotation::setUnit(QString u)
	{
		if (u.isEmpty()) return;
		_value->setUnit(u);
		this->appendProperty("Unit", u);
	}

	QString Rotation::getUnit()
	{
		return _value->getUnit();
	}

	void Rotation::setValue(double* v)
	{
		_value->setValue(v[0], v[1], v[2]);
	}

	void Rotation::setValue(QString x, QString y, QString z)
	{
		_value->setValue(x, y, z);
	}

	void Rotation::getValue(double* v)
	{
		return _value->getValue(v);
	}
	
	void Rotation::getValue(QStringList& list)
	{
		_value->getValue(list);
	}

}
