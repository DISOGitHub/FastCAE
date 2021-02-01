#include "GetModelItemIconFactory.h"
#include "DataProperty/modelTreeItemType.h"
#include "Common.h"


namespace FastCAEDesigner
{
	QIcon GetModelItemIconFactory::Get(int modelItemType)
	{
		if (modelItemType == (int)TreeItemType::ProjectRoot)
			return QIcon(Image_Physics);
		else if (modelItemType == (int)TreeItemType::MeshRoot)
			return QIcon(Image_Mesh);
		else if (modelItemType == (int)TreeItemType::ProjectSimulationSetting)
			return QIcon(Image_Simulation);
		else if (modelItemType == (int)TreeItemType::ProjectSimulationSettingChild)
			return QIcon(Image_Simulation);
		else if (modelItemType == (int)TreeItemType::ProjectBoundaryCondation)
			return QIcon(Image_BC);
		else if (modelItemType == (int)TreeItemType::ProjectBoundaryCondationChild)
			return QIcon(Image_BC);
		else if (modelItemType == (int)TreeItemType::ProjectSolver)
			return QIcon(Image_Solve);
		else if (modelItemType == (int)TreeItemType::ProjectSolverChild)
			return QIcon(Image_Solve);
		else if (modelItemType == (int)TreeItemType::ProjectMonitor)
			return QIcon(Image_Monitor);
		else if (modelItemType == (int)TreeItemType::ProjectMonitorChild)
			return QIcon(Image_Monitor);
		else if (modelItemType == (int)TreeItemType::ProjectPost)
			return QIcon(Image_Post);
		else if (modelItemType == (int)TreeItemType::ProJectPost2DGraph)
			return QIcon(Image_Curve);
		else if (modelItemType == (int)TreeItemType::ProJectPost2DGraphChild)
			return QIcon(Image_Curve);
		else if (modelItemType == (int)TreeItemType::ProjectPost3DGraph)
			return QIcon(Image_Vector);
		else if (modelItemType == (int)TreeItemType::ProjectPost3DGraphChild)
			return QIcon(Image_Vector);
		else
			return QIcon(Image_Physics);
	}
}