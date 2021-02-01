#include "DataConfig.h"
#include "PostCurve.h"
#include <QList>

namespace ConfigOption
{
	DataConfig::~DataConfig()
	{

	}
	void DataConfig::clearData()
	{
		QList<DataProperty::DataBase*> simlist = _simlutationDataHash.values();
		for (int i = 0; i < simlist.size(); ++i)
		{
			DataProperty::DataBase* data = simlist.at(i);
			delete data;
		}
		_simlutationDataHash.clear();

		QList<DataProperty::DataBase*> solList = _solverSettingHash.values();
		for (int i = 0; i < solList.size(); ++i)
		{
			DataProperty::DataBase* d = solList.at(i);
			delete d;
		}
		_solverSettingHash.clear();

		QList<QHash<int, DataProperty::DataBase*>> alldata = _configData.values();
		for (int i = 0; i < alldata.size(); ++i)
		{
			QHash<int, DataProperty::DataBase*> data = alldata.at(i);
			QList<DataProperty::DataBase*> dlist = data.values();
			for (int j = 0; j < dlist.size(); ++j)
			{
				DataProperty::DataBase* d = dlist.at(j);
				if(d != nullptr) delete d;
			}
		}
		_configData.clear();

		QList<QList<PostCurve*>>  curveList = _monitorCurves.values();
		for (QList<PostCurve*> pl : curveList)
		{
			for (PostCurve* c : pl)
				delete c;
		}
		_monitorCurves.clear();
	}
	void DataConfig::appendSimlutationData(ProjectTreeType t, DataProperty::DataBase* data)
	{
		if (_simlutationDataHash.contains(t))
		{
			DataProperty::DataBase* d = _simlutationDataHash.value(t);
			delete d;
			_simlutationDataHash.remove(t);
		}
		_simlutationDataHash[t] = data;
	}
	DataProperty::DataBase* DataConfig::getSimlutationData(ProjectTreeType t)
	{
		return _simlutationDataHash.value(t);
	}
	void DataConfig::appendSolverSettingData(ProjectTreeType t, DataProperty::DataBase* data)
	{
		if (_solverSettingHash.contains(t))
		{
			DataProperty::DataBase* d = _solverSettingHash.value(t);
			delete d;
			_solverSettingHash.remove(t);
		}
		_solverSettingHash[t] = data;
	}
	DataProperty::DataBase* DataConfig::getSolverSettingData(ProjectTreeType t)
	{
		return _solverSettingHash.value(t);
	}
	void DataConfig::appendConfigData(ProjectTreeType type, int id, DataProperty::DataBase* d)
	{
		if (d == nullptr) return;
		DataProperty::DataBase* data = this->getConfigData(type, id);
		if (data != nullptr) delete data;

		_configData[type][id] = d;
	}
	DataProperty::DataBase* DataConfig::getConfigData(ProjectTreeType t, int id)
	{
		if (!_configData.contains(t)) return nullptr;
		QHash<int, DataProperty::DataBase*> proplist = _configData.value(t);
		if (!proplist.contains(id)) return nullptr;

		return proplist.value(id);
	}
	QHash<int, DataProperty::DataBase*> DataConfig::getConfigData(ProjectTreeType t)
	{
		if (_configData.contains(t))
			return _configData.value(t);
		return QHash<int, DataProperty::DataBase*>();
	}
// 	void DataConfig::appendMonitorFile(ProjectTreeType type, QString file)
// 	{
// 		if (_monitorFile.contains(type))
// 		{
// 			QList<DataProperty::DataBase*> monitorlist = _monitorFile.value(type);
// 			for (int i = 0; i < monitorlist.size(); ++i)
// 			{
// 				DataProperty::DataBase* d = monitorlist.at(i);
// 				delete d;
// 			}
// 			_monitorFile.remove(type);
// 		}
// 		_monitorFile.insert(type, monitor);
// 		_monitorFile[type].append(file);
// 	}
	QStringList DataConfig::getMonitorFile(ProjectTreeType type)
	{
		if (_monitorCurves.contains(type))
		{
			QStringList files;
			QList<PostCurve*> curvelist = _monitorCurves.value(type);
			for (int i = 0; i < curvelist.size(); ++i)
			{
				PostCurve* c = curvelist.at(i);
				QString fileName = c->getFile();
				if (!files.contains(fileName))
					files.append(fileName);
			}
			return files;
		}
		else
			return QStringList();
	}
	void DataConfig::appendMonitorCurves(ProjectTreeType type, PostCurve* curve)
	{
// 		if (_monitorCurves.contains(type))
// 		{
// 			QList<PostCurve*> curves = _monitorCurves.value(type);
// 			for (int i = 0; i < curves.size(); ++i)
// 			{
// 				PostCurve* c = curves.at(i);
// 				delete c;
// 			}
// 			_monitorCurves.remove(type);
// 		}
		_monitorCurves[type].append(curve);
	}
	QList<PostCurve*> DataConfig::getMonitorCurves(ProjectTreeType type)
	{
		if (_monitorCurves.contains(type))
		{
			return _monitorCurves.value(type);
		}
		else
 			return QList<PostCurve*>();
	}

}