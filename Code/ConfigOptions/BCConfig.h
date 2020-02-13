#ifndef _BCCONFIG_H_
#define _BCCONFIG_H_

#include "ConfigOptionsAPI.h"
#include <QStringList>
#include <QString>
#include <QList>
#include "DataProperty/modelTreeItemType.h"
#include <QHash>

namespace BCBase
{
	class BCUserDef;
}

namespace ConfigOption
{

	class CONFIGOPTIONSAPI BCConfig
	{
	public:
		BCConfig() = default;
		~BCConfig();

		void clearData();

		BCBase::BCUserDef* getBCByName(QString name, ProjectTreeType t);
		void appendBC(BCBase::BCUserDef* bc, ProjectTreeType t);
		int getBCCount(ProjectTreeType t);
		BCBase::BCUserDef* getBCAt(const int index, ProjectTreeType t);
		void appendEnableType(QString t, ProjectTreeType type);
		bool isEnable(QString type, ProjectTreeType t);
		QStringList getEnabledType(ProjectTreeType t);

	private:

		QHash<ProjectTreeType, QList<BCBase::BCUserDef*>> _bcList{};
		QHash<ProjectTreeType, QStringList> _enableType{};
	};

}



#endif