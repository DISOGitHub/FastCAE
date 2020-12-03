#include "Constant.h"
#include "Value.h"
#include <QDomElement>

namespace Geant4 
{
	Constant::Constant()
	{
		_dim = 1;
		_type = DataType::ConstantType;
		_value = new Value;
	}

	Constant::~Constant()
	{
		if (_value != nullptr)  delete _value;
	}

	void Constant::readGDML(QDomElement & ele)
	{
		QString name = ele.attribute("name");
		this->setName(name);
		
		QString value = ele.attribute("value");
		this->setValue(value);
	}

	void Constant::writeGDML(QDomDocument& doc, QDomElement& ele)
	{
		QDomElement con = doc.createElement("constant");
		QString name = this->getName();
		con.setAttribute("name", name);
		
		QString  v = _value->getReference();
		if (!_value->isReference())
			v = QString::number(_value->getValue());

		con.setAttribute("value", v);
		ele.appendChild(con);
	}

	void Constant::setValue(QString v)
	{
		if (v.isEmpty()) return;
		bool ok = false;
		double d = v.toDouble(&ok);
		if (ok) _value->setValue(d);
		else _value->setReferance(v);
		
		this->appendProperty("Value", v);
	}

	void Constant::getValue(double* v)
	{
		double d = _value->getValue();
		v[0] = d;
	}

	QString Constant::getValue()
	{
		if (_value->isReference())
			return  _value->getReference();
		else
			return QString::number(_value->getValue());
	}

// 	QString Constant::getExpression()
// 	{
// 		return QString();
// 	//	return _value;
// 	}

}