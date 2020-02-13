#include "SolverModel.h"
#include "EditorDescripttionSetup.h"
#include "DataProperty/modelTreeItemType.h"


#pragma execution_character_set("utf-8")


namespace FastCAEDesigner
{
	SolverModel::SolverModel(QString nameEng, QString nameChn, QString iconName, QObject *parent)
		:CustomParameterModel(nameEng, nameChn, iconName, TreeItemType::ProjectSolver, parent)
	{

	}

	SolverModel::~SolverModel()
	{

	}

}