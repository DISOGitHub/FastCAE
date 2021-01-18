#include "ParameterSelectable.h"
#include <QDomAttr>
#include <QDomText>
#include <QDomElement>
#include <QDomDocument>
#include <QDomNodeList>

namespace DataProperty
{
	ParameterSelectable::ParameterSelectable()
		: ParameterBase(Para_Selectable)
	{

	}
	void ParameterSelectable::setOption(QStringList s)
	{
		_option = s;
	}
	QStringList ParameterSelectable::getOption()
	{
		return _option;
	}
	void ParameterSelectable::setCurrentIndex(const int index)
	{
		if (_currentIndex != index)
		{
			_currentIndex = index;
			emit dataChanged();
		}

	}
	int ParameterSelectable::getCurrentIndex()
	{
		return _currentIndex;
	}
	void ParameterSelectable::writeParameter(QDomDocument* doc, QDomElement* parent)
	{
		ParameterBase::writeParameter(doc, parent);
		QDomAttr attr = doc->createAttribute("CurrentIndex");
		attr.setValue(QString::number(_currentIndex));
		parent->setAttributeNode(attr);

		QString text; 
		for (int i = 0; i < _option.size(); ++i)
		{
			QString s = _option.at(i);
			text.append(s);
			text.append(";");
		}
		text.resize(text.size() - 1);
		QDomElement textEle = doc->createElement("Option");
		QDomText domtext = doc->createTextNode(text);
		textEle.appendChild(domtext);
		parent->appendChild(textEle);
		QDomElement domChinese = doc->createElement("Chinese");
		QDomText textChinese = doc->createTextNode(_chinese.join(";"));
		domChinese.appendChild(textChinese);
		parent->appendChild(domChinese);
	}
	void ParameterSelectable::readParameter(QDomElement* e)
	{
		ParameterBase::readParameter(e);
		QString sci = e->attribute("CurrentIndex");
		_currentIndex = sci.toInt();

		QDomNodeList oplist = e->elementsByTagName("Option");
		if (oplist.size() != 1) return;

		QString sop = oplist.at(0).toElement().text();
		_option = sop.split(";");

		QDomNodeList chlist = e->elementsByTagName("Chinese");
		if (chlist.size() != 1) return;
		QString c = chlist.at(0).toElement().text();
		_chinese = c.split(";");
	}
	void ParameterSelectable::copy(ParameterBase* ori, bool valueOnly)
	{
		ParameterBase::copy(ori, valueOnly);
		ParameterSelectable* p = (ParameterSelectable*)ori;
		if (p == nullptr) return;
		int currentIndex = p->getCurrentIndex();
		this->setCurrentIndex(currentIndex);
		if (valueOnly) return;
		_option = p->getOption();
		_chinese = p->getChinese();
	}

	bool ParameterSelectable::isSameValueWith(ParameterBase* p)
	{
		bool same = false;
		ParameterSelectable* ps = dynamic_cast<ParameterSelectable*>(p);
		if (ps != nullptr)
		{
			const int cindex = ps->getCurrentIndex();
			if (cindex == _currentIndex)
				same = true;
		}
		return same;
	}

	QString ParameterSelectable::valueToString()
	{
		return _option.at(_currentIndex);
	}

	QStringList ParameterSelectable::getChinese()
	{
	//	return _chinese;
		return _option;
	}
	void ParameterSelectable::setValueFromString(QString v)
	{
		int i = v.toInt();
		setCurrentIndex(i);
	}
}