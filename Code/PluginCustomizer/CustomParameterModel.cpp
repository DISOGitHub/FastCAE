#include "CustomParameterModel.h"
#include "EditorDescripttionSetup.h"

#pragma execution_character_set("utf-8")


namespace FastCAEDesigner
{
	CustomParameterModel::CustomParameterModel(QString nameEng, QString nameChn, QString iconName, int type, QObject *parent)
		:ModelBase(nameEng, nameChn, iconName, type, parent)
	{

	}

	CustomParameterModel::~CustomParameterModel()
	{

	}

	int CustomParameterModel::ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent)
	{
		FastCAEDesigner::EditorDescripttionSetup dlg(treeItem, this, parent);
		return dlg.exec();
	}

	
}