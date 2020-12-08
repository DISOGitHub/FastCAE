#include "BoundaryModel.h"
#include "EditorBoundaryModel.h"
#include "DataProperty/modelTreeItemType.h"


#pragma execution_character_set("utf-8")


namespace FastCAEDesigner
{
	BoundaryModel::BoundaryModel(QString nameEng, QString nameChn, QString iconName, QObject *parent)
		:ModelBase(nameEng, nameChn, iconName, TreeItemType::ProjectBoundaryCondation, parent)
	{

	}

	BoundaryModel::~BoundaryModel()
	{

	}

	int BoundaryModel::ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent)
	{
		FastCAEDesigner::EditorBoundaryModel dlg(treeItem, this, parent);
		return dlg.exec();
	}

	//获取常用条件
	QString BoundaryModel::GetCommanCondition()
	{
		return _commonCondition;
	}

	void BoundaryModel::SetCommonCondition(QString condition)
	{
		_commonCondition = condition;
	}
	
}