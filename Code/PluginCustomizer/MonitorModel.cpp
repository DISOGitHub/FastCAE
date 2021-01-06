#include "MonitorModel.h"
#include "ModelBase.h"
#include "EditorNameValue.h"
#include "Post2DCurveModel.h"
#include "CreateChildModelFactory.h"
#include "DataProperty/modelTreeItemType.h"
#include <QDebug>
#include "EditorNameValue.h"


#pragma execution_character_set("utf-8")


namespace FastCAEDesigner
{
	MonitorModel::MonitorModel(QString nameEng, QString nameChn, QString iconName, QObject *parent)
		:ModelBase(nameEng, nameChn, iconName, TreeItemType::ProjectMonitor, parent)
	{

	}

	MonitorModel::~MonitorModel()
	{

	}
	//创建子节点
	ModelBase* MonitorModel::CreateChildModel(QString fileName, QObject* parent)
	{
		return CreateChildModelFactory::Create(TreeItemType::ProjectMonitor, fileName, parent);
	}
	//创建子节点
	ModelBase* MonitorModel::CreateChildModel(QTreeWidgetItem* parentTreeItem)
	{
		//qDebug() << "MonitorModel::CreateChildModel(QTreeWidgetItem* parentTreeItem)";

		QList<QString> nameUsedList = getNameUsedList();
		
		EditorNameValue dlg(this);
		dlg.SetUsedNameList(nameUsedList);
		
		int r = dlg.exec();

		if (r == QDialog::Rejected)
			return nullptr;

		ModelBase* childModel = CreateChildModel(dlg.GetNameString(), this);

		if (nullptr == childModel)
			return  nullptr;

		childModel->SetIsEdit(true);
		AddNode(childModel);
		QTreeWidgetItem *treeItem = childModel->CreateTreeItem(parentTreeItem);

		return nullptr;
	}
	//获取已经使用过的名称
	QList<QString> MonitorModel::getNameUsedList()
	{
		QList<QString> nameList;
		QList<ModelBase*> childList = this->GetChildList();
		
		if (childList.count() == 0)
			return nameList;

		for (int i = 0; i < childList.count(); i++)
		{
			ModelBase* model = childList.at(i);

			if (nullptr == model)
				continue;

			nameList.append(model->getFileName());
		}

		return nameList;
	}

}