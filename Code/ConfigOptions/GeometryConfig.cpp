#include "GeometryConfig.h"

namespace ConfigOption
{
	//2019.11.20 xvxinwei Geometry modeling operations sketch
	void GeometryConfig::enableGeometryModeling(bool on)
	{
		_isGeoModeling = on;
	}
	bool GeometryConfig::isGeometryModelingEnabled()
	{
		return _isGeoModeling;
	}
	void GeometryConfig::enableGeometryOperations(bool on)
	{
		_isGeoOperations = on;
	}
	bool GeometryConfig::isGeometryOperationsEnabled()
	{
		return _isGeoOperations;
	}
	void GeometryConfig::enableCreateSketch(bool on)
	{
		_isCreateSketch = on;
	}
	bool GeometryConfig::isCreateSketchEnabled()
	{
		return _isCreateSketch;
	}
	//设置导入几何格式
	void GeometryConfig::setImportSuffix(QString s)
	{
		_importSuffix = s;
	}
	//获取导入几何格式
	QString GeometryConfig::getImportSuffix()
	{
		return _importSuffix;
	}
	//设置导出几何格式
	void GeometryConfig::setExportSuffix(QString s)
	{
		_exportSuffix = s;
	}
	//获取导出几何格式
	QString GeometryConfig::getExportSuffix()
	{
		return _exportSuffix;
	}


	void GeometryConfig::clearData()
	{
		_isCreateSketch = _isGeoModeling = _isGeoOperations = false;
		_importSuffix.clear();
		_exportSuffix.clear();
	}

	bool GeometryConfig::isGeometryEnabled()
	{
		bool in = this->isImportGeometryEnabled();
		bool ex = this->isExportGeometryEnabled();

		bool op = _isCreateSketch || _isGeoModeling || _isGeoOperations;

		return in || ex || op;
	}

	bool GeometryConfig::isImportGeometryEnabled()
	{
		return !_importSuffix.isEmpty();
	}

	bool GeometryConfig::isExportGeometryEnabled()
	{
		return !_exportSuffix.isEmpty();
	}

}