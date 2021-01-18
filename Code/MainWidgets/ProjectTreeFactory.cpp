#include "ProjectTreeFactory.h"
#include "ConfigOptions/ProjectTreeConfig.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ProjectTreeExtend/ProjectTreeExtend.h"

namespace MainWidget
{
	QHash<int, CREATETREE> ProjectTreeFactory::_typeFunHash = QHash<int, CREATETREE>();

	void ProjectTreeFactory::registerType(int type, CREATETREE f)
	{
		_typeFunHash[type] = f;
	}

	void ProjectTreeFactory::removeType(int type)
	{
		_typeFunHash.remove(type);
	}


	ProjectTree::ProjectTreeBase* ProjectTreeFactory::creatTree(int treeType, GUI::MainWindow* m)
	{
		ConfigOption::ProjectTreeConfig* treeconfig = ConfigOption::ConfigOption::getInstance()->getProjectTreeConfig();
		if (treeconfig->isContains((ProjectTreeType)treeType))
		{
			ConfigOption::ProjectTreeInfo* treeTemplate = treeconfig->getProjectTree((ProjectTreeType)treeType);
			ProjectTree::ProjectTreeBase* tree = new ProjectTree::ProjectTreeExtend(m);
			tree->copy(treeTemplate);
			return tree;
		}
		else
		{
			auto FUN = _typeFunHash.value(treeType);
			if (FUN == nullptr) return nullptr;
			QPair<int, ProjectTree::ProjectTreeBase*> ptree{};
			bool ok = FUN(treeType, m, &ptree);
			if (!ok) return nullptr;
			return ptree.second;
		}
		return nullptr;
	}


}