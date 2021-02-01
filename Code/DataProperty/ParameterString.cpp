#include "ParameterString.h"
#include <QDomAttr>
#include <QDomDocument>
#include <QDomElement>

namespace DataProperty
{
	ParameterString::ParameterString()
		: ParameterBase(Para_String)
	{
	}
	
	void ParameterString::setValue(QString s)
	{
		if (s != _value)
		{
			_value = s;
			emit dataChanged();
		}
		
	}

	QString ParameterString::getValue()
	{
		return _value;
	}

	void ParameterString::writeParameter(QDomDocument* doc, QDomElement* parent)
	{
		ParameterBase::writeParameter(doc, parent);
		QDomAttr attr = doc->createAttribute("Text");
		attr.setValue(_value);
		parent->setAttributeNode(attr);
	}
	void ParameterString::readParameter(QDomElement* e)
	{
		ParameterBase::readParameter(e);
		QString s = e->attribute("Text");
		_value = s;
	}
	void ParameterString::copy(ParameterBase* ori, bool valueOnly)
	{
		ParameterBase::copy(ori, valueOnly);

		ParameterString* p = dynamic_cast<ParameterString*>(ori);
		if (p == nullptr) return;
		QString value = p->getValue();
		this->setValue(value);
	}

	bool ParameterString::isSameValueWith(ParameterBase* p)
	{
		bool same = false;
		ParameterString* ps = dynamic_cast<ParameterString*>(p);
		if (ps != nullptr)
		{
			QString v = ps->getValue();
			if (v == _value)
				same = true;
		}
		return same;
	}

	QString ParameterString::valueToString()
	{
		return _value;
	}

	void ParameterString::setValueFromString(QString v)
	{
		setValue(v);
	}

}
