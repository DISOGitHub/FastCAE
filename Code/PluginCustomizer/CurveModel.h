#ifndef CURVEMODEL_H
#define CURVEMODEL_H

#include "ModelBase.h"
#include "Common.h"

namespace FastCAEDesigner
{

	class CurveModel : public ModelBase
	{
		Q_OBJECT
	public:
		CurveModel(QString nameEng, QString nameChn, QString iconName, QObject *parent);
		~CurveModel();
		virtual ModelBase* CreateChildModel(QTreeWidgetItem* parentTreeItem);
		QList<QString> getNameUsedList();
		ModelBase* CreateChildModel(QString fileName, QObject* parent);
	};
}

#endif