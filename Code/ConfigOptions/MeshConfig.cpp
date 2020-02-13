#include "MeshConfig.h"

namespace ConfigOption
{

	bool MeshConfig::isMeshEnabled()
	{
		bool in = this->isImportMeshEnabled();
		bool ex = this->isExportMeshEnabled();

		bool gen = _enableSurfaceMesh || _enableSolidMesh;

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
		return _enableSolidMesh || _enableSurfaceMesh;
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
		
		_importSuffix.clear();
		_exportSuffix.clear();
	
		_enableSurfaceMesh = _enableSolidMesh = false;
		_enableComponent = false;
		_checkMesh	= false;

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

}
