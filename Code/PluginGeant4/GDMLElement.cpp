#include "GDMLElement.h"
#include "Value.h"
#include <QDomElement>
#include <QDomNodeList>
#include <qdebug.h>
namespace Geant4
{
	
	GDMLElement::GDMLElement()
	{
		_value = new Value;
		_value->setUnit(QString("g/mol"));
	}

	GDMLElement::~GDMLElement()
	{
		delete _value;
		_elefractionlist.clear();
	}

	void GDMLElement::setName(const QString& name)
	{
		if (name.isEmpty()) return;
		_name = name;
		this->appendProperty("Name", _name);
	}

	void GDMLElement::setFormula(QString formula)
	{
		if (formula.isEmpty()) return;
		_formula = formula;
		this->appendProperty("Formula",formula);

	}

	QString GDMLElement::getFormula()
	{
		return _formula;

	}

	void GDMLElement::setElementZ(QString Z)
	{
		if (Z.isEmpty()) return;
		_ElementZ = Z;
		this->appendProperty("Z", Z);

	}

	QString GDMLElement::getElementZ()
	{
		return _ElementZ;
	}

	void GDMLElement::setType(QString type)
	{
		if (type.isEmpty()) return;
		_type = type;
		this->appendProperty("Type", type);
	}

	QString GDMLElement::getType()
	{
		return _type;
	}

	QList<Fraction> GDMLElement::getEleFracList()
	{
		return _elefractionlist;
	}

	void GDMLElement::appendEleFracList(Fraction frac)
	{
		_elefractionlist.append(frac);
	}

	void GDMLElement::clearElement()
	{
		_elefractionlist.clear();
	}

	void GDMLElement::writeGDML(QDomDocument& doc, QDomElement& materils)
	{
		QDomElement element = doc.createElement("element");
		element.setAttribute("name", this->getName());
		element.setAttribute("formula", this->getFormula());
		element.setAttribute("Z", this->getElementZ());

		QDomElement atom = doc.createElement("atom");
		atom.setAttribute("type", this->getType());
		atom.setAttribute("value", this->getValue());
		element.appendChild(atom);

		materils.appendChild(element);
	}

	void GDMLElement::setUnit(QString unit)
	{
		if (unit.isEmpty()) return;
		_value->setUnit(unit);
		this->appendProperty("Unit", unit);
	}

	QString GDMLElement::getUnit()
	{
		return _value->getUnit();
	}

	void GDMLElement::setValue(double v)
	{
		_value->setValue(v);
		this->appendProperty("Value", v);
	}

	double GDMLElement::getValue()
	{
		return _value->getValue();
	}


	void GDMLElement::readGDML(QDomElement& ele)
	{
		QString name = ele.attribute("name");
		QString f = ele.attribute("formula");
		QString z = ele.attribute("Z");
		this->setName(name);
		this->setFormula(f);
		this->setElementZ(z);

		QDomNodeList chillist = ele.childNodes();
		for (int j = 0; j < chillist.size(); j++)
		{
			QDomNode onechild = chillist.at(j);
			QString nodename = onechild.nodeName();
			QDomElement elechild = onechild.toElement();
			if (nodename == "atom")
			{
				QString type = elechild.attribute("type");
				QString unit = elechild.attribute("unit");
				double v = elechild.attribute("value").toDouble();
				this->setType(type);
				if (!unit.isEmpty())
				{
					this->setUnit(unit);
				}
				this->setValue(v);
			}
			else if (nodename == "fraction")
			{
				Fraction frac;
				frac.n = elechild.attribute("n").toDouble();
				frac.ref = elechild.attribute("ref");
				this->appendEleFracList(frac);
			}
		}

	}

}


