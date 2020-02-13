#include "ParameterGroup.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>

namespace DataProperty
{
	void ParameterGroup::setDescribe(QString des)
	{
		_describe = des;
	}
	QString ParameterGroup::getDescribe()
	{
		return _describe;
	}

	void ParameterGroup::readParameters(QDomElement* ele)
	{
		_describe = ele->attribute("Describe");
		QString s = ele->attribute("Visible");
		if (s == "false")
			_visible = false;
		ParameterList::readParameters(ele);
	}

	void ParameterGroup::writeParameters(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement groupele = doc->createElement("ParameterGroup");
		groupele.setAttribute("Describe", _describe);
		QString svis = "false";
		if (_visible) svis = "true";
		groupele.setAttribute("Visible", svis);
		ParameterList::writeParameters(doc, &groupele);
		parent->appendChild(groupele);
	}

	void ParameterGroup::copy(ParameterGroup* data)
	{
		_describe = data->getDescribe();
		_visible = data->isVisible();
		//	ParameterList* pl = dynamic_cast<ParameterList*>(data);
		ParameterList::copy(data);
	}

	void ParameterGroup::setVisible(bool v)
	{
		_visible = v;
	}

	bool ParameterGroup::isVisible()
	{
		return _visible;
	}

	void ParameterGroup::copyStates(ParameterGroup* g)
	{
		_visible = g->isVisible();
	}

	void ParameterGroup::dataToStream(QDataStream* datas)
	{
		*datas << _describe << _visible;
		ParameterList::dataToStream(datas);
	}

	void ParameterGroup::appendParameter(ParameterBase* p)
	{
		p->setGroupName(_describe);
		ParameterList::appendParameter(p);
	}

	ParameterBase* ParameterGroup::appendParameter(ParaType type)
	{
		ParameterBase* p = ParameterList::appendParameter(type);
		p->setGroupName(_describe);
		return p;
	}


}