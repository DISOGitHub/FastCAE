#include "PropertyColor.h"

namespace DataProperty
{
	PropertyColor::PropertyColor() : PropertyBase(Prop_Color)
	{
	}
	PropertyColor::PropertyColor(QString name, QColor color) : PropertyBase(Prop_Color)
	{
		_name = name;
		_color = color;
	}
	void PropertyColor::setColor(QColor c)
	{
		_color = c;
	}
	QColor PropertyColor::getColor()
	{
		return _color;
	}
	QVariant PropertyColor::getVariant()
	{
		return _color;
	}

}