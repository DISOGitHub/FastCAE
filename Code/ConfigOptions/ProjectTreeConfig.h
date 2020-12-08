/*******************
Config文件的信息
********************/

#ifndef _PROJECTTREECONFIG_H_
#define _PROJECTTREECONFIG_H_

#include <QMap>
#include <QString>
#include <QStringList>
#include "DataProperty/modelTreeItemType.h"
#include "ConfigOptionsAPI.h"


namespace ConfigOption
{
	class ProjectTreeInfo;

	class CONFIGOPTIONSAPI ProjectTreeConfig
	{
	public:
		ProjectTreeConfig() = default;
		~ProjectTreeConfig();

		void appendTree(ProjectTreeType type, ProjectTreeInfo* ptree);
		ProjectTreeInfo* getProjectTree(ProjectTreeType type);
		QList<ProjectTreeType> getTreeTypes();
		QStringList getTreeNames();
		bool isContains(ProjectTreeType type);
		void clearData();
		QString getNameByType(ProjectTreeType t);
		ProjectTreeType getTypeByName(QString name);
		
	private:
		QMap<ProjectTreeType, ProjectTreeInfo*> _treeHash{};
	};
}


#endif