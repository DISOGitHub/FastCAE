#include "BCVectorBase.h"
#include "DataProperty/ParameterDouble.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomText>
#include <QDomNodeList>
#include <QString>

namespace BCBase
{
	BCVectorBase::BCVectorBase()
	{
		_valueX = new DataProperty::ParameterDouble;
		_valueY = new DataProperty::ParameterDouble;
		_valueZ = new DataProperty::ParameterDouble;
		appendParameter(_valueX);
		appendParameter(_valueY);
		appendParameter(_valueZ);
		_valueX->setAccuracy(4);
		_valueY->setAccuracy(4);
		_valueZ->setAccuracy(4);
	}
	BCVectorBase::BCVectorBase(BCType type,double x, double y, double z)
	{
		_valueX = new DataProperty::ParameterDouble;
		_valueY = new DataProperty::ParameterDouble;
		_valueZ = new DataProperty::ParameterDouble;
		appendParameter(_valueX);
		appendParameter(_valueY);
		appendParameter(_valueZ);
		_valueX->setAccuracy(4);
		_valueY->setAccuracy(4);
		_valueZ->setAccuracy(4);
		setBCType(type);
		setValue(x, y, z);
	}
	BCVectorBase::BCVectorBase(BCType type, double* v)
	{
		_valueX = new DataProperty::ParameterDouble;
		_valueY = new DataProperty::ParameterDouble;
		_valueZ = new DataProperty::ParameterDouble;
		appendParameter(_valueX);
		appendParameter(_valueY);
		appendParameter(_valueZ);
		_valueX->setAccuracy(4);
		_valueY->setAccuracy(4);
		_valueZ->setAccuracy(4);
		setBCType(type);
		setValue(v);
	}
	void BCVectorBase::setVariableName(QString x, QString y, QString z)
	{
		_valueX->setDescribe(x);
		_valueY->setDescribe(y);
		_valueZ->setDescribe(z);
	}
	void BCVectorBase::setValue(double* v)
	{
		_valueX->setValue(v[0]);
		_valueY->setValue(v[1]);
		_valueZ->setValue(v[2]);
	}
	void BCVectorBase::setValue(double x, double y, double z)
	{
		_valueX->setValue(x);
		_valueY->setValue(y);
		_valueZ->setValue(z);
	}
	void BCVectorBase::getValue(double* v)
	{		
		v[0] = _valueX->getValue();
		v[1] = _valueY->getValue();
		v[2] = _valueZ->getValue();
	}
	QDomElement& BCVectorBase::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		BCBase::writeToProjectFile(doc, parent);
		QDomElement xele = doc->createElement("ValueX");
		QDomElement yele = doc->createElement("ValueY");
		QDomElement zele = doc->createElement("ValueZ");
		_valueX->writeParameter(doc, &xele);
		_valueY->writeParameter(doc, &yele);
		_valueZ->writeParameter(doc, &zele);
		
		parent->appendChild(xele);
		parent->appendChild(yele);
		parent->appendChild(zele);

		return *parent;
	}
	void BCVectorBase::readDataFromProjectFile(QDomElement* ele)
	{
		BCBase::readDataFromProjectFile(ele);
		QDomNodeList xlist = ele->elementsByTagName("ValueX");
		if (xlist.size() == 1)
		{
			QDomElement xele = xlist.at(0).toElement();
			_valueX->readParameter(&xele);
		}
		QDomNodeList ylist = ele->elementsByTagName("ValueY");
		if (xlist.size() == 1)
		{
			QDomElement yele = ylist.at(0).toElement();
			_valueY->readParameter(&yele);
		}
		QDomNodeList zlist = ele->elementsByTagName("ValueZ");
		if (zlist.size() == 1)
		{
			QDomElement zele = zlist.at(0).toElement();
			_valueZ->readParameter(&zele);
		}
	}

}