#include "TreeItemData.h"
#include <QString>
#include <QDir>

namespace ConfigOption
{

	void TreeItem::setText(const QString &text)
	{
		_text = text;
	}
	void TreeItem::SetParent(const QString &parent)
	{
		_parent = parent;
	}
	void TreeItem::setType(const QString &type)
	{
		_type = getTreeItemTypeByString(type);
	}
	TreeItemType TreeItem::getType()
	{
		return _type;
	}
	void TreeItem::appendContextMenu(const QString &menu)
	{
		_contextMenu.append(menu);
	}
	QString TreeItem::getParent()
	{
		return _parent;
	}
	QString TreeItem::getText()
	{
		return _text;
	}
	int TreeItem::getContextMenuCount()
	{
		return _contextMenu.size();
	}
	QString TreeItem::getContextMenuAt(const int index)
	{
		if (index>=0 && index<_contextMenu.size())
			return _contextMenu.at(index);
		else return "";
	}
	void TreeItem::setIcon(const QString& icon)
	{
// 		QString path = QDir::currentPath();
// 		QString iconpath = path + QString("/ConigFiles/Icon/")+icon;
// 		_icon = QIcon(iconpath);
		_icon = icon;
	}
	QString TreeItem::getIcon()
	{
		return _icon;
	}
	void TreeItem::setChinese(QString c)
	{
		_chinese = c;
	}
	QString TreeItem::getChinese()
	{
		return _chinese;
	}
	QStringList TreeItem::getContextMenu()
	{
		return _contextMenu;
	}
	void TreeItem::copy(TreeItem* item)
	{
		_text = item->getText();
		_parent = item->getParent();
		_chinese = item->getChinese();
		_icon = item->getIcon();
		_type = item->getType();
		_contextMenu = item->getContextMenu();
		_dataID = item->getDataID();
	}
	void TreeItem::setDataID(int id)
	{
		if (id <= 0) return;
		_dataID = id;
	}
	int TreeItem::getDataID()
	{
		return _dataID;
	}

}