#include "PropertyString.h"

namespace DataProperty
{
	PropertyString::PropertyString() : PropertyBase(Prop_String)
	{
	}
	PropertyString::PropertyString(QString name, QString value) : PropertyBase(Prop_String)
	{
		_name = name;
		_value = value;
	}
	void PropertyString::setValue(QString v)
	{
		_value = v;
	}
	QString PropertyString::getValue()
	{
		return _value;
	}
	QVariant PropertyString::getVariant()
	{
		return _value;
	}
}