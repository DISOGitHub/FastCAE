#include "ParameterInt.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include <QStringList>

namespace DataProperty
{
	ParameterInt::ParameterInt() : ParameterBase(Para_Int)
	{
		_range[0] = -1e6;
		_range[1] = 1e6;
	}
	void ParameterInt::setValue(int v)
	{
		if (v != _value)
		{
			_value = v;
			emit dataChanged();
		}
		
	}
	int ParameterInt::getValue()
	{
		return _value;
	}
	void ParameterInt::getRange(int* range)
	{
		range[0] = _range[0];
		range[1] = _range[1];
	}
	void ParameterInt::setRange(int* range)
	{
		_range[0] = range[0];
		_range[1] = range[1];
	}
	void ParameterInt::setUnit(QString u)
	{
		_unit = u;
	}
	QString ParameterInt::getUnit()
	{
		return _unit;
	}
	void ParameterInt::writeParameter(QDomDocument* doc, QDomElement* parent)
	{
		ParameterBase::writeParameter(doc, parent);
		QDomAttr attr = doc->createAttribute("Value");
		attr.setValue(QString::number(_value));
		parent->setAttributeNode(attr);

		QDomAttr uattr = doc->createAttribute("Unit");
		uattr.setValue(_unit);
		parent->setAttributeNode(uattr);

		QDomElement rangeele = doc->createElement("Range");
		QString sr = QString("%1,%2").arg(_range[0]).arg(_range[1]);
		QDomText rtext = doc->createTextNode(sr);
		rangeele.appendChild(rtext);
		parent->appendChild(rangeele);

	}
	void ParameterInt::readParameter(QDomElement* e)
	{
		ParameterBase::readParameter(e);

		QString sv = e->attribute("Value");
		this->setValue(sv.toInt());
		QString u = e->attribute("Unit");
		this->setUnit(u);

		QDomNodeList ralist = e->elementsByTagName("Range");
		if (ralist.size() != 1) return;
		QString sr = ralist.at(0).toElement().text();
		QStringList slrange = sr.split(",");
		if (slrange.size() != 2) return;
		_range[0] = slrange[0].toInt();
		_range[1] = slrange[1].toInt();
	}

	void ParameterInt::copy(ParameterBase* ori, bool valueOnly)
	{
		ParameterBase::copy(ori, valueOnly);
		ParameterInt* p = dynamic_cast<ParameterInt*>(ori);
		if (p == nullptr) return;
		int v = p->getValue();
		this->setValue(v);
		if (valueOnly) return;
		_unit = p->getUnit();
		p->getRange(_range);
	}

	bool ParameterInt::isSameValueWith(ParameterBase* p)
	{
		bool same = false;
		ParameterInt* pi = dynamic_cast<ParameterInt*>(p);
		if (pi != nullptr)
		{
			const int v = pi->getValue();
			if (v == _value)
				same = true;
		}
		return same;
	}

	QString ParameterInt::valueToString()
	{
		return QString::number(_value);
	}

	void ParameterInt::setValueFromString(QString v)
	{
		int value = v.toInt();
		setValue(value);
	}

}