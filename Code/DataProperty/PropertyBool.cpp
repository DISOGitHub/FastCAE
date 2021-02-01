#include "PropertyBool.h"

namespace DataProperty
{
	PropertyBool::PropertyBool() : PropertyBase(Prop_Bool)
	{
	}
	PropertyBool::PropertyBool(QString name, bool value) : PropertyBase(Prop_Bool)
	{
		_name = name;
		_value = value;
	}
	void PropertyBool::setValue(bool v)
	{
		_value = v;
	}
	bool PropertyBool::getValue()
	{
		return _value;
	}

	QVariant PropertyBool::getVariant()
	{
		return _value;
	}


}