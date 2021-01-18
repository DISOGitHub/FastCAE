#ifndef _PROJECTTREECON__H__
#define _PROJECTTREECON__H__

#include "ProjectTreeExtendAPI.h"
#include <QString>
#include <QHash>
#include <QList>
#include <QStringList>

namespace ModelData
{
	class ModelDataBase;
};
namespace ProjectTree
{
	class ProjectTreeBase;
}

typedef void(*CONTECTMENU)(ModelData::ModelDataBase*, ProjectTree::ProjectTreeBase*);

struct RegFunction
{
	int _TreeType = -1;
	QString _desibe{};
	QString _item{};
	CONTECTMENU _fun{};
};

namespace ProjectTree
{
	class PROJECTTREEEXTENDAPI ProjectTreeConfig
	{
	public:
		static void RegisterRootContextMenu(int type, QString des, CONTECTMENU f);
		static void RegisterContextMenu(int type, QString des,QString item, CONTECTMENU f);
		static void removeRootContextMenu(int type, QString des);
		static void removeContextMenu(int type, QString des, QString item);

		static QStringList getRootContextMenuDescribe(int type);
		static QStringList getContextMenuDescribe(int type, QString item);

		static CONTECTMENU getRootContextMenuFunction(int type, QString des);
		static CONTECTMENU getRootContextMenuFunction(int type, QString des, QString item);


	private:
		static QList<RegFunction> _rootFuns;
		static QList<RegFunction> _contextFuns;
		
	};
}


#endif
