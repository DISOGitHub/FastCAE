#include "ParameterBase.h"
#include <QDomElement>
#include <QDomDocument>
#include <QDomAttr>
#include <QDebug>

namespace DataProperty
{
	ParameterBase::ParameterBase(ParaType t):
		_type(t)
	{

	}
	void ParameterBase::setDescribe(QString s)
	{
		_describe = s;
	}
	QString ParameterBase::getDescribe()
	{
		return _describe;
	}
	void ParameterBase::setParaType(ParaType t)
	{
		_type = t;
	}
	ParaType ParameterBase::getParaType()
	{
		return _type;
	}
	void ParameterBase::writeParameter(QDomDocument* doc, QDomElement* parent)
	{
//		QDomElement paraEle = doc->createElement("Parameter");
		
		QDomAttr tyattr = doc->createAttribute("Type");
		QString stype = ParaTypeToString(_type);
		tyattr.setValue(stype);
		parent->setAttributeNode(tyattr);

		QDomAttr desAttr = doc->createAttribute("Describe");
		desAttr.setValue(_describe);
		parent->setAttributeNode(desAttr);

		QDomAttr chineAttr = doc->createAttribute("Chinese");
		chineAttr.setValue(_chinese);
		parent->setAttributeNode(chineAttr);

		QDomAttr editattr = doc->createAttribute("Editable");
		QString attrs = "false";
		if (_editable) attrs = "true";
		editattr.setValue(attrs);
		parent->setAttributeNode(editattr);

		QDomAttr visattr = doc->createAttribute("Visible");
		QString vis = "false";
		if (_visible) vis = "true";
		visattr.setValue(vis);
		parent->setAttributeNode(visattr);

	}
	void ParameterBase:: readParameter(QDomElement* e)
	{
		QString des = e->attribute("Describe");
		this->setDescribe(des);
		QString chinese = e->attribute("Chinese");
		this->setChinese(chinese);
		QString edit = e->attribute("Editable");
		if (edit.toLower() == "false")
			_editable = false;
		QString visible = e->attribute("Visible");
		if (visible.toLower() == "false")
			_visible = false;
	}
	bool ParameterBase::isEditable()
	{
		return _editable;
	}

	QString ParameterBase::ParaTypeToString(ParaType t)
	{
		QString stype;
		
		switch (t)
		{
		case DataProperty::Para_Int: stype = "Int"; break;
		case DataProperty::Para_Double: stype = "Double"; break;
		case DataProperty::Para_Color: stype = "Color"; break;
		case DataProperty::Para_String: stype = "String"; break;
		case DataProperty::Para_Bool: stype = "Bool"; break;
		case DataProperty::Para_Selectable: stype = "Selectable"; break;
		case DataProperty::Para_Path: stype = "Path"; break;
		case DataProperty::Para_Table: stype = "Table"; break;
		default: break;
		}
		
		return stype;
	}
	ParaType ParameterBase::StringToParaType(QString stype)
	{
		ParaType t = Para_Blank;
		
		if (stype == "Int") t = Para_Int;
		else if (stype == "Double") t = Para_Double;
		else if (stype == "Color") t = Para_Color;
		else if (stype == "String") t = Para_String;
		else if (stype == "Bool") t = Para_Bool;
		else if (stype == "Selectable") t = Para_Selectable;
		else if (stype == "Path") t = Para_Path;
		else if (stype == "Table") t = Para_Table;

		return t;
	}

	void ParameterBase::copy(ParameterBase* ori, bool valueOnly)
	{
		if (valueOnly) return;
		_type = ori->getParaType();
		_describe = ori->getDescribe();
		_editable = ori->isEditable();
		_visible = ori->isVisible();
		_chinese = ori->getChinese();
	}
	void ParameterBase::setVisible(bool v)
	{
		_visible = v;
	}
	bool ParameterBase::isVisible()
	{
		return _visible;
	}

	void ParameterBase::setEditable(bool e)
	{
		_editable = e;
	}

	bool ParameterBase::isSameValueWith(ParameterBase* p)
	{
		return false;
	}

	void ParameterBase::copyStatus(ParameterBase* p)
	{
//		qDebug() << _describe << "   " << p->getDescribe();
		_editable = p->isEditable();
		_visible = p->isVisible();
//		qDebug() << _visible;
	}

	void ParameterBase::appendObserver(ConfigOption::ParameterObserver* obs)
	{
		if (!_observerList.contains(obs))
			_observerList.append(obs);
	}

	QList<ConfigOption::ParameterObserver*> ParameterBase::getObserverList()
	{
		return _observerList;
	}

	QString ParameterBase::valueToString()
	{
		qDebug() << "paraBase, not override";
		return QString();
	}

	void ParameterBase::setChinese(QString chinese)
	{
		_chinese = chinese;
	}

	QString ParameterBase::getChinese()
	{
		return _chinese;
	}

	void ParameterBase::setModuleType(ModuleType t)
	{
		_moduleType = t;
	}

	DataProperty::ModuleType ParameterBase::getModuleType()
	{
		return _moduleType;
	}

	int ParameterBase::getDataID()
	{
		return _dataID;
	}

	void ParameterBase::setDataID(int id)
	{
		_dataID = id;
	}

	int ParameterBase::getDataIndex()
	{
		return _dataIndex;
	}

	void ParameterBase::setDataIndex(int index)
	{
		_dataIndex = index;
	}

	QString ParameterBase::getGroupName()
	{
		return _groupName;
	}

	void ParameterBase::setGroupName(QString group)
	{
		_groupName = group;
	}

	QString ParameterBase::genAbsoluteName()
	{
		QString name;
		if (_groupName.isEmpty())
		{
			name = _describe;
		}
		else
		{
			name = QString("%1/%2").arg(_groupName).arg(_describe);
		}
		return name;
	}

	void ParameterBase::setValueFromString(QString v)
	{

	}





}


