#include <Value.h>
#include <QRegExp>
#include <math.h>
#include <QDebug>
#include "DataManager.h"
#include "DefineManager.h"
#include "PreDefineBase.h"

namespace Geant4
{
	Value::Value(double v)
	{
		_value = v;
	}

	Value::~Value()
	{

	}

	double Value::getValue()
	{
		if(!_isRef)
			return _value;
		auto ma = DataManager::getInstance()->getDefineManager();
		PreDefineBase* pb = ma->getPreDefineByName(_constantRef);
		if (pb == nullptr)
		{
			return transformPI();
		}
		double p[3] = { 0 };
		pb->getValue(p);
		return p[0];
	}


	void Value::setValue(double v)
	{
		_value = v;
	}

	void Value::setName(QString name)
	{
		_name = name;
	}

	QString Value::getName()
	{
		return _name;
	}

	void Value::setUnit(QString unit)
	{
		_unit = unit;
	}
	QString Value::getUnit()
	{
		return _unit;
	}


	void Value::setReferance(QString ref)
	{
		_isRef = true;
		_constantRef = ref;
	}

	QString Value::getReference()
	{
		return _constantRef;
	}

	void Value::isReference(bool is)
	{
		_isRef = is;
	}

	bool Value::isReference()
	{
		return _isRef;
	}

	void Value::setType(QString t)
	{
		_type = t;
	}

	QString Value::getType()
	{
		return _type;
	}

	double Value::transformPI()
	{
		QString text = _constantRef.toLower();
		if (!text.contains("pi")) return  0;
		qDebug() << text;

		QRegExp rx("[/\*+-]");
		int index = rx.indexIn(text);
		if (index < 0) return 0;
		QString c = text.at(index);
		QStringList sl = text.split(c);
		if (sl.size() != 2) return 0;
		QString first = sl.at(0).simplified();
		QString second = sl.at(1).simplified();
		if (first.endsWith(".")) first.append("0");
		if (second.endsWith(".")) second.append("0");


		double f = 0;
		double s = 0;

		if (first == "pi") f = 3.141592653;
		else f = first.toDouble();
		if (second == "pi")  s = 3.141592653;
		else s = second.toDouble();

		if (c == "*")
			return f * s;
		if (c == "/" || c =="\\")
		{
			if (fabs(s) < 1e-6) return 0;
			return f / s;
		}
		if (c == "+")
			return f + s;
		if (c == "-")
			return f - s;
		return 0;
	}

}




