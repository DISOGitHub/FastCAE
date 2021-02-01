#include "GeometryConfig.h"

#define GEOON true;
#define GEOOFF false;
#define GEOIMPORT  "stp;step;igs;iges;brep"
#define GEOEXPORT  "stp;step;igs;iges;brep"

namespace ConfigOption
{
	GeometryConfig::GeometryConfig()
	{
		this->clearData();
	}

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



	void GeometryConfig::enableGeometryEdit(bool on)
	{
		_isGeometryEdit = on;
	}

	bool GeometryConfig::isGeometryEditEnabled()
	{
		return _isGeometryEdit;
	}

	void GeometryConfig::enableGeometryCreateSet(bool on)
	{
		_isGeoCreateSet = on;
	}

	bool GeometryConfig::isGeometryCreateSetEnabled()
	{
		return _isGeoCreateSet;
	}

	void GeometryConfig::enableMeasureDsitance(bool on)
	{
		_isMeasureDistance = on;
	}

	bool GeometryConfig::isMeasureDistanceEnabled()
	{
		return _isMeasureDistance;
	}

	void GeometryConfig::clearData()
	{
		_importSuffix = GEOIMPORT;
		_exportSuffix = GEOEXPORT;
		_isGeoModeling = GEOON;
		_isGeoOperations = GEOON;
		_isCreateSketch = GEOON;
		_isGeometryEdit = GEOON;
		_isGeoCreateSet = GEOON;
		_isMeasureDistance = GEOON;

	}

	bool GeometryConfig::isGeometryEnabled()
	{
		bool in = this->isImportGeometryEnabled();
		bool ex = this->isExportGeometryEnabled();

		bool op = _isCreateSketch || _isGeoModeling || _isGeoOperations || _isGeometryEdit || _isGeoCreateSet || _isMeasureDistance;

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