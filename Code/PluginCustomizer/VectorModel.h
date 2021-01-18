#ifndef VECTORMODEL_H
#define VECTORMODEL_H

#include "ModelBase.h"
#include "Common.h"

namespace FastCAEDesigner
{

	class VectorModel : public ModelBase
	{
		Q_OBJECT
	public:
		VectorModel(QString nameEng, QString nameChn, QString iconName, QObject *parent);
		~VectorModel();

		virtual ModelBase* CreateChildModel(QTreeWidgetItem* parentTreeItem);
		//virtual int  ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent);
		QList<QString> getNameUsedList();
		ModelBase* CreateChildModel(QString fileName, QObject* parent);
	};
}

#endif