#ifndef _DATACONFIG_H_
#define _DATACONFIG_H_

#include "ConfigOptionsAPI.h"
#include "DataProperty/DataBase.h"
#include "DataProperty/modelTreeItemType.h"
#include <QHash>

namespace DataProperty
{
	class DataBase;
}

namespace ConfigOption
{
	class PostCurve;

	class CONFIGOPTIONSAPI DataConfig
	{
	public:
		DataConfig() = default;
		~DataConfig();

		void appendSimlutationData(ProjectTreeType t, DataProperty::DataBase* data);
		DataProperty::DataBase* getSimlutationData(ProjectTreeType t);
		
		void appendSolverSettingData(ProjectTreeType t, DataProperty::DataBase* data);
		DataProperty::DataBase* getSolverSettingData(ProjectTreeType t);
		void appendConfigData(ProjectTreeType type, const int id, DataProperty::DataBase* d);
		DataProperty::DataBase* getConfigData(ProjectTreeType type, int id);
		QHash<int, DataProperty::DataBase*> getConfigData(ProjectTreeType t);

//		void appendMonitorFile(ProjectTreeType type);
		QStringList getMonitorFile(ProjectTreeType type);
		void appendMonitorCurves(ProjectTreeType type, PostCurve* curves);
		QList<PostCurve*> getMonitorCurves(ProjectTreeType type);

		void clearData();

	private:
		QHash<ProjectTreeType, DataProperty::DataBase*> _simlutationDataHash{};
		QHash<ProjectTreeType, DataProperty::DataBase*> _solverSettingHash{};
		QHash<ProjectTreeType, QHash<int, DataProperty::DataBase*>> _configData{};
//		QHash<ProjectTreeType, q> _monitorFile{};
		QHash<ProjectTreeType, QList<PostCurve*>> _monitorCurves{};

	};
}


#endif