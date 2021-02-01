#include "modelTreeItemType.h"
#include <assert.h>

TreeItemType getTreeItemTypeByString(const QString& typeString)
{
	TreeItemType type = Undefined;

	if (typeString == "SimulationSetting") type = ProjectSimulationSetting;
	else if (typeString == "SimulationSettingChild") type = ProjectSimulationSettingChild;
	else if (typeString == "SimulationSettingGrandSon") type = ProjectSimulationSettingGrandSon;
	else if (typeString == "Geometry") type = ProjectGeometry;
	else if (typeString == "Mesh") type = ProjectComponent;
	else if (typeString == "Solver") type = ProjectSolver;
	else if (typeString == "PostSetting") type = ProjectPostSetting;

	else if (typeString == "GeometryChild") type = ProjectGeometryChild;
	else if (typeString == "MeshChild") type = ProjectComponentChild;
	else if (typeString == "SolverChild") type = ProjectSolverChild;
	else if (typeString == "SolverSettingGrandSon") type = ProjectSolverGrandSon;
	else if (typeString == "PostSettingChild") type = ProjectPostSettingChild;
	
	else if (typeString == "Post") type = ProjectPost;
	else if (typeString == "Post3D") type = ProjectPost3DGraph;
	else if (typeString == "Post2D") type = ProJectPost2DGraph;
	else if (typeString == "Post2DChild") type = ProJectPost2DGraphChild;
	else if (typeString == "Post3DChild") type = ProjectPost3DGraphChild;

	return type;
}


ProjectTreeType getTreeTypeByString(const QString& typestring)
{
	ProjectTreeType type = UnDefined;

	QString index = typestring.toLower().remove("type");
	bool ok = false;
	int i = index.toInt(&ok);
	if (!ok)
	{
	//	assert(0);
		return type;
	}
	return  ProjectTreeType(TreeType + i);

}

QString getTreeTypeToString(ProjectTreeType type)
{
	QString stype = "Undefined";
	if (int(type) > 0)
	{
		stype = QString("Type%1").arg((int)type);
	}
	return stype;
}