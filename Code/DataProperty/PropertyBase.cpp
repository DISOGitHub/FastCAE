#include "PropertyBase.h"

namespace DataProperty
{ 
	PropertyBase::PropertyBase(PropertyType type)
	{
		_type = type;
	}
	void PropertyBase::setType(PropertyType tp)
	{
		_type = tp;
	}
	PropertyType PropertyBase::getType()
	{
		return _type;
	}
	void PropertyBase::setName(const QString &name)
	{
		_name = name;
	}
	QString PropertyBase::getName()
	{
		return _name;
	}
	void PropertyBase::setEditable(bool visable)
	{
		_editable = visable;
	}
	bool PropertyBase::isEditable()
	{
		return _editable;
	}

	QVariant PropertyBase::getVariant()
	{
		return QString();
	}

}