#include "VectorModel.h"
#include "ModelBase.h"
#include "EditorNameValue.h"
#include "Post3DFileModel.h"
#include "CreateChildModelFactory.h"
#include "DataProperty/modelTreeItemType.h"
#include <QDebug>

#pragma execution_character_set("utf-8")


namespace FastCAEDesigner
{
	VectorModel::VectorModel(QString nameEng, QString nameChn, QString iconName, QObject *parent)
		:ModelBase(nameEng, nameChn, iconName, TreeItemType::ProjectPost3DGraph, parent)
	{

	}

	VectorModel::~VectorModel()
	{

	}
	//创建子节点
	ModelBase* VectorModel::CreateChildModel(QString fileName, QObject* parent)
	{
		return CreateChildModelFactory::Create(TreeItemType::ProjectPost3DGraph, fileName, parent);
	}
	//创建子节点
	ModelBase* VectorModel::CreateChildModel(QTreeWidgetItem* parentTreeItem)
	{
	//	qDebug() << "MonitorModel::CreateChildModel(QTreeWidgetItem* parentTreeItem)";

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
	//获取使用过的名字
	QList<QString> VectorModel::getNameUsedList()
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