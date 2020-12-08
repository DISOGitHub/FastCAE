#include "SimulationModel.h"
#include "EditorDescripttionSetup.h"
#include "DataProperty/modelTreeItemType.h"
#include <QTreeWidgetItem>


#pragma execution_character_set("utf-8")


namespace FastCAEDesigner
{
	SimulationModel::SimulationModel(QString nameEng, QString nameChn, QString iconName, QObject *parent)
		:CustomParameterModel(nameEng, nameChn, iconName, TreeItemType::ProjectSimulationSetting, parent)
	{

	}

	SimulationModel::~SimulationModel()
	{

	}

}