 /***************************
存储配置文件中树节点信息
libaojun
*****************************/

#ifndef TREEITEMDATA_H
#define TREEITEMDATA_H

#include <QString>
#include <QStringList>
#include <QIcon>
#include "DataProperty/modelTreeItemType.h"
#include "ConfigOptionsAPI.h"

namespace ConfigOption
{
	class CONFIGOPTIONSAPI TreeItem
	{
	public:
		TreeItem() = default;
		~TreeItem() = default;

		void copy(TreeItem* item);

		void setText(const QString &text);
		void setType(const QString &type);
		void SetParent(const QString &parent);
		void appendContextMenu(const QString &menu);
		QString getText();
		QString getParent();
		TreeItemType getType();
		int getContextMenuCount();
		QString getContextMenuAt(const int index);
		void setIcon(const QString& icon);
		QString getIcon();
		void setChinese(QString c);
		QString getChinese();
		QStringList getContextMenu();
		void setDataID(int id);
		int getDataID();

	private:
		QString _text{};
		QString _parent{};
		QString _chinese{};
		QString _icon{};
		TreeItemType _type{TreeItemType::Undefined};
		QStringList _contextMenu{};
		int _dataID{ -1 };
	};


}


#endif
