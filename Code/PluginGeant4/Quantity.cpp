#include "Quantity.h"
#include "Value.h"
#include <QDomElement>

namespace Geant4 
{

	Quantity::Quantity()
	{
		_value = new Value;
		_dim = 1;
		_type = DataType::QuantityType;
	}

	Quantity::~Quantity()
	{
		delete _value;
	}

	void Quantity::readGDML(QDomElement & ele)
	{
		QString name = ele.attribute("name");
		this->setName(name);

		QString type = ele.attribute("type");
		this->setType(type);

		QString unit = ele.attribute("unit");
		this->setUnit(unit);

		double value = ele.attribute("value").toDouble();
		this->setValue(value);
	}

	void Quantity::writeGDML(QDomDocument& doc, QDomElement& ele)
	{
		QDomElement qua = doc.createElement("quantity");
		QString name = this->getName();
		QString type = _value->getType();
		QString unit = _value->getUnit();
		QString value = _value->getReference();

		qua.setAttribute("name", name);
		qua.setAttribute("type", type);
		qua.setAttribute("unit", unit);
		if (value.isEmpty())
			qua.setAttribute("value", _value->getValue());
		else
			qua.setAttribute("value", value);
		ele.appendChild(qua);
	}

	void Quantity::setValue(double v)
	{
		_value ->setValue(v);
		this->appendProperty("Value", v);
	}

	void Quantity::setValue(QString s)
	{
		bool ok = false;
		double v = s.toDouble(&ok);
		if (ok || s.isEmpty())
		{
			_value->setValue(v);
			this->appendProperty("Value", v);
		}
		else
		{
			_value->setReferance(s);
			this->appendProperty("Value", s);
		}
	}

	void Quantity::getValue(double* v)
	{
		double v0 = _value->getValue();
		v[0] = v0;
	}

	QString Quantity::getValueString()
	{
		QString v = _value->getReference();
		if (v.isEmpty())
			return QString::number(_value->getValue());
		else
			return v;
	}

	void Quantity::setUnit(QString u)
	{
		if (u.isEmpty()) return;
		_value->setUnit(u);
		this->appendProperty("Unit", u);
	}

	QString Quantity::getUnit()
	{
		return _value->getUnit();
	}

	void Quantity::setType(QString t)
	{
		if (t.isEmpty()) return;
		_value->setType(t);
		this->appendProperty("Type", t);
	}

	QString Quantity::getType()
	{
		return _value->getType();
	}

}