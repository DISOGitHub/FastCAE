#include "PostModel.h"
#include "ModelBase.h"
#include "DataProperty/modelTreeItemType.h"


#pragma execution_character_set("utf-8")


namespace FastCAEDesigner
{
	PostModel::PostModel(QString nameEng, QString nameChn, QString iconName, QObject *parent)
		:ModelBase(nameEng, nameChn, iconName, TreeItemType::ProjectPost, parent)
	{

	}

	PostModel::~PostModel()
	{

	}
}