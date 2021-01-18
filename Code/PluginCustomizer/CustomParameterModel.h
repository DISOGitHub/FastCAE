#ifndef CUSTOMPARAMETERMODEL_H
#define CUSTOMPARAMETERMODEL_H

#include "ModelBase.h"
#include "Common.h"

namespace FastCAEDesigner
{
	class CustomParameterModel : public ModelBase
	{
		Q_OBJECT
	public:
		CustomParameterModel(QString nameEng, QString nameChn, QString iconName, int type, QObject *parent);
		~CustomParameterModel();
		virtual int ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent);
	};
}

#endif