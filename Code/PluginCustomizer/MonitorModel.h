#ifndef MONITORMODEL_H
#define MONITORMODEL_H

#include "ModelBase.h"
#include <QTreeWidgetItem>


namespace FastCAEDesigner
{

	class MonitorModel : public ModelBase
	{
		Q_OBJECT
	public:
		MonitorModel(QString nameEng, QString nameChn, QString iconName, QObject *parent);
		~MonitorModel();

		virtual ModelBase* CreateChildModel(QTreeWidgetItem* parentTreeItem);
		///virtual int ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent);
		QList<QString> getNameUsedList();
		ModelBase* CreateChildModel(QString fileName, QObject* parent);

	};
}

#endif