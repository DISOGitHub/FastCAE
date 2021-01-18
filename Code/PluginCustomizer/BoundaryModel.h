#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "ModelBase.h"
#include "Common.h"

namespace FastCAEDesigner
{

	class BoundaryModel : public ModelBase
	{
		Q_OBJECT
	public:
		BoundaryModel(QString nameEng, QString nameChn, QString iconName, QObject *parent);
		~BoundaryModel();
		virtual int ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent);
		QString GetCommanCondition();               //获取常用条件
		void SetCommonCondition(QString condition); //设置边界

	private:
		QString _commonCondition;
	};
}

#endif