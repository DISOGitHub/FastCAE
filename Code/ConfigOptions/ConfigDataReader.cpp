#include "ConfigDataReader.h"
#include "ConfigDataBase.h"
#include "BCConfigReader.h"
#include "DataConfigReader.h"
#include "GlobalConfigReader.h"
#include "ObserverConfigReader.h"
#include "TreeConfigReader.h"
#include "SolverConfig.h"
#include "MaterialConfig.h"
//#include "NodeFormConfigReader.h"

namespace ConfigOption
{
	ConfigDataReader::ConfigDataReader(QString path, ConfigDataBase* data) :
		_path(path), _data(data)
	{
	}

	QString ConfigDataReader::read()
	{
		QString error;
		bool ok = false;
		_data->clearAllConfig();

		BCConfigReader bcreader(_path + "/BCConfig.config", _data->getBCConfig());
		ok = bcreader.read();
		if (!ok) error.append("BCConfig,");

		DataConfigReader datareader(_path + "/DataConfig.config", _data->getDataConfig(), _data->getPostConfig());
		ok = datareader.read();
		if (!ok) error.append("DataConfig,");

		GlobalConfigReader golbalreader(_path + "/GlobalConfig.config",_data);
		ok = golbalreader.read();
		if (!ok) error.append("GlobalConfig,");

		ObserverConfigReader observerreader(_path + "/ObserverConfig.config",_data->getObseverConfig());
		ok = observerreader.read();
		if (!ok) error.append("ObserverConfig,");

		TreeConfigReader reader(_path + "/treeConfig.config", _data->getProjectTreeConfig());
		ok = reader.read();
		if (!ok) error.append("treeConfig,");

		ok = _data->getSolverOption()->read(_path + "/SolverConfig.config");
		if (!ok) error.append("SolverConfig,");

		ok = _data->getMaterialConfig()->readConfig(_path + "MaterialConfig.config");
		if (!ok) error.append("MaterialConfig,");

// 		NodeFormConfigReader formReader("C:\\Users\\Administrator\\Desktop\\FastCAEWidget.ui",
// 										_data->getNodeFormConfig());
//		ok = formReader.read();


		return error;
	}

	
}



