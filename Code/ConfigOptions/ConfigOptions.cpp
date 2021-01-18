#include "ConfigOptions.h"
#include "BCConfig.h"
#include "BCConfigReader.h"
#include "DataConfig.h"
#include "DataConfigReader.h"
#include "GlobalConfig.h"
#include "GeometryConfig.h"
#include "MeshConfig.h"
#include "SolverConfig.h"
#include "GlobalConfigReader.h"
#include "PostConfig.h"
#include "ObserverConfig.h"
#include "ObserverConfigReader.h"
#include "MaterialConfig.h"
#include "ProjectTreeConfig.h"
//#include "NodeFormConfig.h"
#include "TreeConfigReader.h"

#include <QCoreApplication>

namespace ConfigOption
{
	ConfigOption* ConfigOption::_instance = nullptr;

	ConfigOption* ConfigOption::getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new ConfigOption;
		}
		return _instance;
	}

	ConfigOption::ConfigOption()
	{
//		_treeConfig = new ProjectTreeConfig;
		_bcConfig = new BCConfig;
		_dataConfig = new DataConfig;
		_globalConfig = new GlobalConfig;
		_geoConfig = new GeometryConfig;
		_meshConfig = new MeshConfig;
		_postConfig = new PostConfig;
		_solverOption = new SolverOption;
		_observerConfig = new ObserverConfig;
		_materialConfig = new MaterialConfig;
		_projectTreeConfig = new ProjectTreeConfig;
//		_nodeFormConfig = new NodeFormConfig;
	}

	ConfigOption::~ConfigOption()
	{
//		if (_treeConfig != nullptr ) delete _treeConfig;
		if (_bcConfig != nullptr) delete _bcConfig;
		if (_dataConfig != nullptr) delete _dataConfig;
		if (_globalConfig != nullptr) delete _globalConfig;
		if (_geoConfig != nullptr) delete _geoConfig;
		if (_meshConfig != nullptr) delete _meshConfig;
		if (_postConfig != nullptr) delete _postConfig;
		if (_solverOption != nullptr) delete _solverOption;
		if (_materialConfig != nullptr) delete _materialConfig;
		if (_projectTreeConfig != nullptr) delete _projectTreeConfig;
//		if (_nodeFormConfig != nullptr) delete _nodeFormConfig;
	}
	
	void ConfigOption::clearAllConfig()
	{
//		_treeConfig->clearData();
		_bcConfig->clearData();
		_dataConfig->clearData();
		_globalConfig->clearData();
		_geoConfig->clearData();
		_meshConfig->clearData();
		_postConfig->clearData();
		_solverOption->clearData();
		_observerConfig->clearData();
		_materialConfig->clearData();
		_projectTreeConfig->clearData();
		//_nodeFormConfig->clearData();
	}

	BCConfig* ConfigOption::getBCConfig()
	{
		return _bcConfig;
	}

	DataConfig* ConfigOption::getDataConfig()
	{
		return _dataConfig;
	}

	GeometryConfig* ConfigOption::getGeometryConfig()
	{
		return _geoConfig;
	}

	GlobalConfig* ConfigOption::getGlobalConfig()
	{
		return _globalConfig;
	}

	MeshConfig* ConfigOption::getMeshConfig()
	{
		return _meshConfig;
	}

	PostConfig* ConfigOption::getPostConfig()
	{
		return _postConfig;
	}

	SolverOption* ConfigOption::getSolverOption()
	{
		return _solverOption;
	}

	bool ConfigOption::isGeometryEnabled()
	{
		return _geoConfig->isGeometryEnabled();
	}

	bool ConfigOption::isMeshEnabled()
	{
		return _meshConfig->isMeshEnabled();
	}

	bool ConfigOption::isPostEnabled()
	{
		return _postConfig->isPostEnabled();
	}

	ObserverConfig* ConfigOption::getObseverConfig()
	{
		return _observerConfig;
	}

	MaterialConfig* ConfigOption::getMaterialConfig()
	{
		return _materialConfig;
	}

	ProjectTreeConfig* ConfigOption::getProjectTreeConfig()
	{
		return _projectTreeConfig;
	}

// 	NodeFormConfig* ConfigOption::getNodeFormConfig()
// 	{
// 		return _nodeFormConfig;
// 	}
}