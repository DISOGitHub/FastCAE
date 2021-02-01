#include "PropertyDouble.h"

namespace DataProperty
{
	PropertyDouble::PropertyDouble() : PropertyBase(Prop_Double)
	{
	}
	PropertyDouble::PropertyDouble(QString name, double value) : PropertyBase(Prop_Double)
	{
		_name = name;
		_value = value;
	}
	void PropertyDouble::setValue(double v)
	{
		_value = v;
	}
	double PropertyDouble::getValue()
	{
		return _value;
	}
	QVariant PropertyDouble::getVariant()
	{
		return _value;
	}

}