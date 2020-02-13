#include "BCUserDef.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QDir>
#include <QDebug>

namespace BCBase
{
	BCUserDef::BCUserDef()
	{
		this->setType(UserDef);
	}

	QString BCUserDef::getName()
	{
		return _name;
	}
	void BCUserDef::setName(QString s)
	{
		_name = s;
//		appendProperty(QObject::tr("Name"), _name);
	}
	QDomElement& BCUserDef::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		BCBase::writeToProjectFile(doc, parent);
		QDomAttr attr = doc->createAttribute("Name");
		attr.setValue(_name);
		parent->setAttributeNode(attr);
// 		QString icon = _icon.name();
// 		qDebug() << icon;
		BCBase::writeParameters(doc, parent);

		return *parent;
	}
	void BCUserDef::setIcon(QString icon)
	{
//		QString path = QDir::currentPath();
		_icon = icon;
		
	}
	QString BCUserDef::getIcon()
	{
		return _icon;
	}
	void BCUserDef::readDataFromProjectFile(QDomElement*e)
	{
		BCBase::readDataFromProjectFile(e);
		_name = e->attribute("Name");
		QString icon = e->attribute("Icon");
		this->setIcon(icon);
		DataBase::readParameters(e);
	}
	void BCUserDef::copy(DataBase* data)
	{
		_name = ((BCUserDef*)data)->getName();
		_icon = ((BCUserDef*)data)->getIcon();
		BCBase::copy(data);
//		setName(_name);
	}
}