#ifndef _MODELTREEITEMTYPE_H_
#define _MODELTREEITEMTYPE_H_

#include <QString>
#include "DataPropertyAPI.h"

enum ProjectTreeType
{
	UnDefined = -1,
	TreeType = 0,
	PluginDefType = 100,
	TreeTypeBoundary = 100000,
};


enum TreeItemType  
{
	Undefined = -1,
	GeometryRoot = 1,
	GeometryChild,
	Datum,
	DatumPlane,
	GeoComponentRoot,
	GeoComponentChild,
	MeshRoot,
	MeshChild,
	MeshSetRoot,
	MeshSetChild,
	MaterialRoot,
	MaterialChild,
	PhyaicsModelRoot,

	ProjectRoot,
	ProjectSimulationSetting,
	ProjectSimulationSettingChild,
	ProjectSimulationSettingGrandSon,
	ProjectGeometry,
	ProjectGeometryChild,
	ProjectComponent,
	ProjectComponentChild,
	ProjectBoundaryCondation,
	ProjectBoundaryCondationChild,
	ProjectMonitor,
	ProjectMonitorChild,
	ProjectSolver,
	ProjectSolverChild,
	ProjectSolverGrandSon,
	ProjectPostSetting,
	ProjectPostSettingChild,
	ProjectCaseTemplate,


	ProjectPost,
	ProjectPost3DGraph,
	ProjectPost3DGraphChild,
	ProJectPost2DGraph,
	ProJectPost2DGraphChild,
	ProjectPostCounter,
	ProjectPostCounterChild,
	ProjectPostVector,
	ProjectPostVectorChild,
	ProjectPostStreamLine,
	ProjectPostStreamLineChild,

	ProjectReport,
	ProjectReportChild,

	SelfDefineItem,

};



extern TreeItemType DATAPROPERTYAPI getTreeItemTypeByString(const QString& type);

extern ProjectTreeType DATAPROPERTYAPI getTreeTypeByString(const QString& type);

extern QString DATAPROPERTYAPI getTreeTypeToString(ProjectTreeType ty);


#endif
