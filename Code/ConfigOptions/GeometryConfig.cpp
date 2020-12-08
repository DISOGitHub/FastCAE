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
	//���õ��뼸�θ�ʽ
	void GeometryConfig::setImportSuffix(QString s)
	{
		_importSuffix = s;
	}
	//��ȡ���뼸�θ�ʽ
	QString GeometryConfig::getImportSuffix()
	{
		return _importSuffix;
	}
	//���õ������θ�ʽ
	void GeometryConfig::setExportSuffix(QString s)
	{
		_exportSuffix = s;
	}
	//��ȡ�������θ�ʽ
	QString GeometryConfig::getExportSuffix()
	{
		return _exportSuffix;
	}



	void GeometryConfig::clearData()
	{
		_importSuffix = GEOIMPORT;
		_exportSuffix = GEOEXPORT;
		_isGeoModeling = GEOON;
		_isGeoOperations = GEOON;
		_isCreateSketch = GEOON;
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