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
		QString GetCommanCondition();               //��ȡ��������
		void SetCommonCondition(QString condition); //���ñ߽�

	private:
		QString _commonCondition;
	};
}

#endif