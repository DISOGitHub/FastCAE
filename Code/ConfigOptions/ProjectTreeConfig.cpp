#include "ProjectTreeConfig.h"
#include "ProjectTreeInfo.h"


namespace ConfigOption
{

	ProjectTreeConfig::~ProjectTreeConfig()
	{
		clearData();
	}
	void ProjectTreeConfig::appendTree(ProjectTreeType type, ProjectTreeInfo* ptree)
	{
		if (!_treeHash.contains(type))
		{
			_treeHash[type] = ptree;
		}
	}
	ProjectTreeInfo* ProjectTreeConfig::getProjectTree(ProjectTreeType type)
	{
		if (_treeHash.contains(type))
		{
			return _treeHash.value(type);
		}
		return nullptr;
	}
	QStringList ProjectTreeConfig::getTreeNames()
	{
		QList<ProjectTreeInfo*> trees = _treeHash.values();
		QStringList names;
		for (int i = 0; i < trees.size();++i)
		{
			names.append(trees.at(i)->getName());
		}
		return names;
	}
	bool ProjectTreeConfig::isContains(ProjectTreeType type)
	{
		return _treeHash.contains(type);
	}

	QList<ProjectTreeType> ProjectTreeConfig::getTreeTypes()
	{
		return _treeHash.keys();
	}
	void ProjectTreeConfig::clearData()
	{
		QList<ProjectTreeInfo*> values = _treeHash.values();
		const int n = values.size();
		for (int i = 0; i < n; ++i)
		{
			ProjectTreeInfo* tree = values.at(i);
			tree->clearTreeItems();
			delete tree;
		}
		_treeHash.clear();
	}

	QString ProjectTreeConfig::getNameByType(ProjectTreeType t)
	{
		QString name;
		ProjectTreeInfo* tree = _treeHash.value(t);
		if (tree != nullptr)
		{
			name = tree->getName();
		}
		return name;
	}

	ProjectTreeType ProjectTreeConfig::getTypeByName(QString name)
	{
		QList<ProjectTreeInfo*> trees = _treeHash.values();
		ProjectTreeType t = UnDefined;
		for (int i = 0; i < trees.size(); ++i)
		{
			auto tree = trees.at(i);
			QString tname = tree->getName();
			if (name == tname)
			{
				t = _treeHash.key(tree);
				break;
			}
		}
		return t;
	}

}