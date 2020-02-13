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
		_value = s;
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
	void ParameterString::copy(ParameterBase* ori)
	{
		ParameterBase::copy(ori);

		ParameterString* p = (ParameterString*)ori;
		_value = p->getValue();
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
		_value = v;
	}

}
