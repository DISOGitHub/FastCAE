#include "CreateChildModelFactory.h"
#include "ModelBase.h"
#include "DataProperty/modelTreeItemType.h"
#include <QObject>
#include "CustomParameterModel.h"
#include "Post2DCurveModel.h"
#include "Post3DFileModel.h"

namespace FastCAEDesigner
{
	ModelBase* CreateChildModelFactory::Create(int parentModelType, QString nameEng, QString nameChn, QString icon, QObject *parent)
	{
		if (parentModelType == TreeItemType::ProjectRoot)
			return nullptr;

		if (parentModelType == TreeItemType::ProjectComponent)
			return nullptr;
		
		if (parentModelType == TreeItemType::ProjectSimulationSetting)
			return new CustomParameterModel(nameEng, nameChn, icon, TreeItemType::ProjectSimulationSettingChild, parent);

		if (parentModelType == TreeItemType::ProjectBoundaryCondation)
			return new CustomParameterModel(nameEng, nameChn, icon, TreeItemType::ProjectBoundaryCondationChild, parent);

		if (parentModelType == TreeItemType::ProjectSolver)
			return new CustomParameterModel(nameEng, nameChn, icon, TreeItemType::ProjectSolverChild, parent);

		if (parentModelType == TreeItemType::ProjectMonitor)
			return new Post2DCurveModel(nameEng, nameChn, icon, TreeItemType::ProjectMonitorChild, parent);

		if (parentModelType == TreeItemType::ProjectPost)
			return nullptr;

		if (parentModelType == TreeItemType::ProJectPost2DGraph)
			return new Post2DCurveModel(nameEng, nameChn, icon, TreeItemType::ProJectPost2DGraphChild, parent);

		if (parentModelType == TreeItemType::ProjectPost3DGraph)
			return new Post3DFileModel(nameEng, nameChn, icon,TreeItemType::ProjectPost3DGraphChild, parent);

		if (parentModelType == TreeItemType::ProjectSolverChild)
			return new CustomParameterModel(nameEng, nameChn, icon, TreeItemType::ProjectSolverGrandSon, parent);

		if (parentModelType == TreeItemType::ProjectSimulationSettingChild)
			return new CustomParameterModel(nameEng, nameChn, icon, TreeItemType::ProjectSimulationSettingGrandSon, parent);

		return nullptr;

	}

	ModelBase* CreateChildModelFactory::Create(int parentModelType, QString fileName, QObject *parent)
	{
		if (parentModelType == TreeItemType::ProjectMonitor)
			return new Post2DCurveModel(fileName, TreeItemType::ProjectMonitorChild, parent);

		if (parentModelType == TreeItemType::ProJectPost2DGraph)
			return new Post2DCurveModel(fileName, TreeItemType::ProJectPost2DGraphChild, parent);

		if (parentModelType == TreeItemType::ProjectPost3DGraph)
			return new Post3DFileModel(fileName, TreeItemType::ProjectPost3DGraphChild, parent);

		return nullptr;
	}
}