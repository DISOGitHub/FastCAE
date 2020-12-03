#include "PropertyList.h"
#include "PropertyBase.h"
#include "PropertyInt.h"
#include "PropertyDouble.h"
#include "PropertyString.h"
#include "PropertyColor.h"
#include "PropertyBool.h"
#include "PropertyPoint.h"
#include <QString>
#include <QDebug>
#include <assert.h>

namespace DataProperty
{
	PropertyList::~PropertyList()
	{
		clearPropertyList();
	}
	void PropertyList::clearPropertyList()
	{
		for (int i = 0; i < _propertyList.size(); ++i)
		{
			auto p = _propertyList.at(i);
			delete p;
		}
		_propertyList.clear();
	}
	int PropertyList::getPropertyCount()
	{
		return _propertyList.size();
	}
	PropertyBase* PropertyList::getPropertyAt(const int index)
	{
		assert(index >= 0 && index < _propertyList.size());
		return _propertyList.at(index);
	}
	PropertyBase* PropertyList::getPropertyByName(const QString &name)
	{
		const int n = _propertyList.size();
		for (int i = 0; i < n; ++i)
		{
			auto p = _propertyList.at(i);
			const QString pname = p->getName();
			if (pname == name)
				return p;
		}
		return nullptr;
	}
	bool PropertyList::appendProperty(PropertyBase* p)
	{
		const int n = _propertyList.size();
		for (int i = 0; i < n; ++i)
		{
			auto pop = _propertyList.at(i);
			PropertyType type = pop->getType();
			QString name = pop->getName().toLower();
			if ((p->getType() == type) && (p->getName().toLower() == name)) //如果存在就更新
			{
				if (type == Prop_INT)
				{
					int data = ((DataProperty::PropertyInt*)p)->getValue();
					((DataProperty::PropertyInt*)pop)->setValue(data);
				}
				else if (type == Prop_Double)
				{
					double data = ((DataProperty::PropertyDouble*)p)->getValue();
					((DataProperty::PropertyDouble*)pop)->setValue(data);
				}
				else if (type == Prop_String)
				{
					QString string = ((DataProperty::PropertyString*)p)->getValue();
					((DataProperty::PropertyString*)pop)->setValue(string);
				}
				else if (type == Prop_Color)
				{
					QColor color = ((DataProperty::PropertyColor*)p)->getColor();
					((DataProperty::PropertyColor*)pop)->setColor(color);
				}
				else if (type == Prop_Bool)
				{
					bool b = ((DataProperty::PropertyBool*)p)->getValue();
					((DataProperty::PropertyBool*)pop)->setValue(b);
				}
				else if (type == Prop_Point)
				{
					double c[3];
					((DataProperty::PropertyPoint*)p)->getValue(c);
				}
				return false;
			}
				
		}
		_propertyList.append(p);
		return true;
	}
	void PropertyList::appendProperty(QString name, int value)
	{
		PropertyInt* pi = new PropertyInt(name, value);
		if (!appendProperty(pi))
			delete pi;
	}
	void PropertyList::appendProperty(QString name, double value)
	{
		PropertyDouble* pd = new PropertyDouble(name, value);
		if (!appendProperty(pd))
			delete pd;
	}
	void PropertyList::appendProperty(QString name, QString string)
	{
		PropertyString* ps = new PropertyString(name, string);
		if (!appendProperty(ps))
			delete ps;
	}
	void PropertyList::appendProperty(QString name, QColor color)
	{
		PropertyColor* pc = new PropertyColor(name, color);
		if (!appendProperty(pc))
			delete pc;
	}
	void PropertyList::appendProperty(QString name, bool value)
	{
		PropertyBool *pb = new PropertyBool(name, value);
		if (!appendProperty(pb))
			delete pb;

	}
	void PropertyList::appendProperty(QString name, double *p)
	{
		PropertyPoint* pp = new PropertyPoint(name, p);
		if (!appendProperty(pp))
			delete pp;		
	}
	void PropertyList::appendProperty(QString name, double x, double y, double z)
	{
		PropertyPoint* pp = new PropertyPoint(name, x, y, z);
		if (!appendProperty(pp))
			delete pp;
	}

	bool PropertyList::removeProperty(QString name)
	{		
		auto p = getPropertyByName(name);
		if (_propertyList.contains(p))
		{
			bool result = _propertyList.removeOne(p);
			delete p;
			p = nullptr;
			return result;
		}
		else return false;
	}

	void PropertyList::copy(PropertyList* propList)
	{
		if (propList == nullptr) return;
		const int n = propList->getPropertyCount();
		for (int i = 0; i < n; ++i)
		{
			auto pop = propList->getPropertyAt(i);
			PropertyType type = pop->getType();
			QString name = pop->getName();
			PropertyBase* prop = nullptr;
			switch (type)
			{
			case DataProperty::Prop_Blank:
				break;
			case DataProperty::Prop_INT:
				prop = new PropertyInt;
//				const int v = ((PropertyInt*)pop)->getValue();
				((PropertyInt*)prop)->setValue(((PropertyInt*)pop)->getValue());
				break;
			case DataProperty::Prop_Double:
				prop = new PropertyDouble;
//				const double d = ((PropertyDouble*)pop)->getValue();
				((PropertyDouble*)prop)->setValue(((PropertyDouble*)pop)->getValue());
				break;
			case DataProperty::Prop_Color:
				prop = new PropertyColor;
//				const QColor c = ((PropertyDouble*)pop)->getValue();
				((PropertyColor*)prop)->setColor(((PropertyDouble*)pop)->getValue());
				break;
			case DataProperty::Prop_String:
				prop = new PropertyString;
//				const QString s = ((PropertyString*)pop)->getValue();
				((PropertyString*)prop)->setValue(((PropertyString*)pop)->getValue());
				break;
			case DataProperty::Prop_Bool:
				prop = new PropertyBool;
//				const bool b = ((PropertyBool*)pop)->getValue();
				((PropertyBool*)prop)->setValue(((PropertyBool*)pop)->getValue());
				break;
			case DataProperty::Prop_Point:
				break;
			case DataProperty::Prop_IDList:
				break;
			default: 
				break;
			}

			if (prop != nullptr)
			{
				prop->setName(name);
                if (!appendProperty(prop))
                    delete prop;
			}
				
		}
		
	}

}