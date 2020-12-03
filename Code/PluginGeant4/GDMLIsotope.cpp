#include "GDMLIsotope.h"
#include "Value.h"
#include <QDomElement>
#include <QDomNodeList>

namespace Geant4
{
	GDMLIsotope::GDMLIsotope()
	{
		_value = new Value;
		_value->setUnit(QString("g/mol"));
	}

	GDMLIsotope::~GDMLIsotope()
	{
		if (_value != nullptr) delete _value;
	}

	void GDMLIsotope::setName(const QString & name)
	{
		_name = name;
		this->appendProperty("Name", _name);
	}

	void GDMLIsotope::setIsotopeN(QString N)
	{
		_IsotopeN = N;
		this->appendProperty("N", N);

	}

	QString GDMLIsotope::getIsotopeN()
	{
		return _IsotopeN;
	}

	void GDMLIsotope::setIsotopeZ(QString Z)
	{
		_IsotopeZ = Z;
		this->appendProperty("Z", Z);
	}

	QString GDMLIsotope::getIsotopeZ()
	{
		return _IsotopeZ;
	}

	void GDMLIsotope::setType(QString type)
	{
		if (type.isEmpty()) return;
		_type = type;
		this->appendProperty("Type", type);
	}

	QString GDMLIsotope::getType()
	{
		return _type;
	}

	void GDMLIsotope::setUnit(QString unit)
	{
		if (unit.isEmpty()) return;
		_value->setUnit(unit);
		this->appendProperty("Unit", unit);
	}

	QString GDMLIsotope::getUnit()
	{
		return _value->getUnit();
	}

	void GDMLIsotope::setValue(double v)
	{
		_value->setValue(v);
		this->appendProperty("Value", v);
	}

	double GDMLIsotope::getValue()
	{
		return _value->getValue();
	}

	void GDMLIsotope::writeGDML(QDomDocument& doc, QDomElement& materils)
	{
		QDomElement isotope = doc.createElement("isotope");
		isotope.setAttribute("name", this->getName());
		isotope.setAttribute("N", this->getIsotopeN());
		isotope.setAttribute("Z", this->getIsotopeZ());

		QDomElement atom = doc.createElement("atom");
		atom.setAttribute("type", this->getType());
		atom.setAttribute("value", this->getValue());
		isotope.appendChild(atom);

		materils.appendChild(isotope);
	}

	void GDMLIsotope::readGDML(QDomElement & ele)
	{
		QString name = ele.attribute("name");
		QString z = ele.attribute("Z");
		QString n = ele.attribute("N");
		this->setName(name);
		this->setIsotopeN(n);
		this->setIsotopeZ(z);
		QDomNodeList chillist = ele.childNodes();
		for (int j = 0; j < chillist.size(); j++)
		{
			QDomNode onechild = chillist.at(j);
			QDomElement elechild = onechild.toElement();
			QString type = elechild.attribute("type");
			QString unit = elechild.attribute("unit");
			if (!unit.isEmpty())
			{
				this->setUnit(unit);
			}
			double value = elechild.attribute("value").toDouble();
			this->setType(type);
			this->setValue(value);
		}
	}

}


