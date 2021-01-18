#ifndef _PROJECTTREEFACTORY_H_
#define _PROJECTTREEFACTORY_H_

#include "mainWidgetsAPI.h"
#include <QHash>
#include <QPair>

namespace GUI
{
	class MainWindow;
}

namespace ProjectTree
{
	class ProjectTreeBase;
}

typedef bool(*CREATETREE)(int, GUI::MainWindow*, QPair<int,ProjectTree::ProjectTreeBase*>* );

namespace MainWidget
{
	class MAINWIDGETSAPI ProjectTreeFactory
	{
	public:
		ProjectTreeFactory() = default;
		~ProjectTreeFactory() = default;

		static void registerType(int type, CREATETREE f);
		static void removeType(int type);
		static ProjectTree::ProjectTreeBase* creatTree(int type, GUI::MainWindow* m);
	
	private:
		static QHash<int, CREATETREE> _typeFunHash;

	};
}


#endif