#include "MeshConfig.h"
#include <QDebug>

#define MESHON  true;
#define MESHOFF false;
#define MESHIMPORT "cgns;msh;neu;stl;dat;vtk;inp"
#define MESHEXPORT "vtk;neu"


namespace ConfigOption
{
	MeshConfig::MeshConfig()
	{
		this->clearData();

	}

	bool MeshConfig::isMeshEnabled()
	{
		bool in = this->isImportMeshEnabled();
		bool ex = this->isExportMeshEnabled();

		bool gen = _enableSurfaceMesh || _enableSolidMesh || _fluidMesh;

		return in || ex || gen ;
	}
	
	bool MeshConfig::isImportMeshEnabled()
	{
		return !_importSuffix.isEmpty();
	}
	
	bool MeshConfig::isExportMeshEnabled()
	{
		return !_exportSuffix.isEmpty();
	}

	void MeshConfig::setImportSuffix(QString s)
	{
		_importSuffix = s;
	}
	
	QString MeshConfig::getImportSuffix()
	{		
		return _importSuffix;
	}

	void MeshConfig::setExportSuffix(QString s)
	{
		_exportSuffix = s;
	}
	 
	QString MeshConfig::getExportSuffix()
	{
		return _exportSuffix;
	}
	
	bool MeshConfig::isMeshGenerationEnabled()
	{
		return _enableSolidMesh || _enableSurfaceMesh || _fluidMesh;
	}
	void MeshConfig::enableSurfaceMesh(bool on)
	{
		_enableSurfaceMesh = on;
	}
	bool MeshConfig::isSurfaceMeshEnabled()
	{
		return _enableSurfaceMesh;
	}
	void MeshConfig::enableSolidMesh(bool on)
	{
		_enableSolidMesh = on;
	}
	bool MeshConfig::isSolidMeshEnabled()
	{
		return _enableSolidMesh;
	}
	void MeshConfig::enableComponent(bool on)
	{
		_enableComponent = on;
	}
	bool MeshConfig::isComponentEnabled()
	{
		return _enableComponent;
	}

	void MeshConfig::clearData()
	{
		
		_exportSuffix = MESHEXPORT;
		_importSuffix = MESHIMPORT;

		_enableSurfaceMesh = MESHON;
		_enableSolidMesh = MESHON;
		_enableComponent = MESHON;
		_checkMesh = MESHON;
		_fluidMesh = MESHON;
		_filterMesh = MESHON;
		_meshModeling = MESHON;
	}

	//设置检测网格质量
	void MeshConfig::setCheckMesh(bool on)
	{
		_checkMesh = on;
	}
	//获取检测网格质量状态
	bool MeshConfig::getCheckMeshEnabled()
	{
		return _checkMesh;
	}

	void MeshConfig::enableFluidMesh(bool on)
	{
		_fluidMesh = on;
	}

	bool MeshConfig::isFluidMeshEnabled()
	{
		return _fluidMesh;
	}

	void MeshConfig::enableFilterMesh(bool on)
	{
		_filterMesh = on;
	}

	bool MeshConfig::isFilterMeshEnabled()
	{
		return _filterMesh;
	}

	void MeshConfig::enableMeshModeling(bool on)
	{
		_meshModeling = on;
	}

	bool MeshConfig::isMeshModelingEnabled()
	{
		return _meshModeling;
	}

}
