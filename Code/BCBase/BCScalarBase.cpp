#include "BCScalarBase.h"
#include "DataProperty/ParameterDouble.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomText>
#include <QDomNodeList>
#include <QString>

namespace BCBase
{
	BCScalarBase::BCScalarBase(BCType type)
	{
		setBCType(type);
		DataProperty::ParameterBase* p = this->appendParameter(DataProperty::Para_Double);
		_value = (DataProperty::ParameterDouble*)p;
	}

	BCScalarBase::BCScalarBase()
	{
		DataProperty::ParameterBase* p = this->appendParameter(DataProperty::Para_Double);
		_value = (DataProperty::ParameterDouble*)p;
	}
	void BCScalarBase::setVariableName(QString s)
	{
		_value->setDescribe(s);
	}
	void BCScalarBase::setValue(double v)
	{
		_value->setValue(v);
	}

	double BCScalarBase::getValue()
	{
		return _value->getValue();
	}
	QDomElement& BCScalarBase::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		BCBase::writeToProjectFile(doc, parent);
		QDomElement valueele = doc->createElement("Value");
		_value->writeParameter(doc, &valueele);
		parent->appendChild(valueele);

		return *parent;
	}
	void BCScalarBase::readDataFromProjectFile(QDomElement* ele)
	{
		BCBase::readDataFromProjectFile(ele);
		QDomNodeList vlist = ele->elementsByTagName("Value");
		if (vlist.size() != 1) return;
		QDomElement vele = vlist.at(0).toElement();
		_value->readParameter(&vele);
	}

}