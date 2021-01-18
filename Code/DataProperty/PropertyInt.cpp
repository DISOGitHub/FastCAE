#include "PropertyInt.h"

namespace DataProperty
{
	PropertyInt::PropertyInt() : PropertyBase(Prop_INT)
	{
	}
	PropertyInt::PropertyInt(QString name, int value) : PropertyBase(Prop_INT)
	{
		_name = name;
		_value = value;
	}
	void PropertyInt::setValue(int v)
	{
		_value = v;
	}
	int PropertyInt::getValue()
	{
		return _value;
	}
	QVariant PropertyInt::getVariant()
	{
		return _value;
	}
}