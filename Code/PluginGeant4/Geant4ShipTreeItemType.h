#ifndef _GEANT4TREEITEMTYPE_H_
#define _GEANT4TREEITEMTYPE_H_

#include <QString>
#include "Geant4PluginAPI.h"


enum Geant4TreeItemType
{
	Geant4Root,
	Geant4DefineRoot,
	Geant4MaterialRoot,
	Geant4SolidRoot,
	Geant4StructureRoot,
	Geant4TrackRoot,
	Geant4Track,
	Geant4Setup,

	Geant4ParameterConfig,
	ParameterDefine,
	ParameterMaterials,
	ParameterSolids,
	ParameterStructure,
	Geant4Post,
	ParameterPosition,
	ParameterRotation,
	ParameterElement,
	ParameterMaterial,
	ParameterIsotope,
	ParameterGeometry,
	ParameterConstant,
	ParameterQuantity,
	ParameterExpression,

	GeoTypeVolume,
	GeoTypePhysvol,

	Geant4Source,
	Geant4Physical,
	Geant4PhysicalEnergyLimit,
	Geant4Geometry,
	Geant4Calculate,

	Geant4PhEnThItem,
	Geant4CaAnItem,
	Geant4CaAnHiItem,
	Geant4CaAnNoItem,

	Geant4CaAnRelationItem,

	Geant4CaAnVoItem,
	Geant4CaAnVoInItem,

	SourceType,
	SourceAngle,
	SourcePara,
};





#endif
