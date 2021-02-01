#include "ProjectTreeConfig.h"


namespace ProjectTree
{
	QList<RegFunction> ProjectTreeConfig::_contextFuns = QList<RegFunction>();
	QList<RegFunction> ProjectTreeConfig::_rootFuns = QList<RegFunction>();

	void ProjectTreeConfig::RegisterRootContextMenu(int type, QString des, CONTECTMENU f)
	{
		RegFunction funs;
		funs._TreeType = type;
		funs._desibe = des;
		funs._fun = f;
		_rootFuns.append(funs);

	}

	void ProjectTreeConfig::RegisterContextMenu(int type, QString des,QString item, CONTECTMENU f)
	{
		RegFunction funs;
		funs._TreeType = type;
		funs._desibe = des;
		funs._item = item;
		funs._fun = f;
		_contextFuns.append(funs);
	}


	void ProjectTreeConfig::removeRootContextMenu(int type, QString des)
	{
		const int n = _rootFuns.size();
		for (int i = 0; i < n; ++i)
		{
			RegFunction f = _rootFuns.at(i);
			if (f._TreeType == type && f._desibe == des)
			{
				_rootFuns.removeAt(i);
				return;
			}
		}
	}

	void ProjectTreeConfig::removeContextMenu(int type, QString des, QString item)
	{
		const int n = _contextFuns.size();
		for (int i = 0; i < n; ++i)
		{
			RegFunction f = _contextFuns.at(i);
			if (f._TreeType == type && f._item == item && f._desibe == des)
			{
				_contextFuns.removeAt(i);
				return;
			}
		}
	}


	QStringList ProjectTreeConfig::getRootContextMenuDescribe(int type)
	{
		QStringList desList;
		const int n = _rootFuns.size();
		for (int i = 0; i < n; ++i)
		{
			RegFunction f = _rootFuns.at(i);
			if (f._TreeType == type)
				desList.append(f._desibe);
		}
		return  desList;
	}

	QStringList ProjectTreeConfig::getContextMenuDescribe(int type, QString item)
	{
		QStringList desList;
		const int n = _contextFuns.size();
		for (int i = 0; i < n; ++i)
		{
			RegFunction f = _contextFuns.at(i);
			if (f._TreeType == type && f._item == item)
			{
				desList.append(f._desibe);
			}
		}
		return desList;
	}

	CONTECTMENU ProjectTreeConfig::getRootContextMenuFunction(int type, QString des)
	{
		const int n = _rootFuns.size();
		for (int i = 0; i < n; ++i)
		{
			RegFunction f = _rootFuns.at(i);
			if (f._TreeType == type && f._desibe == des)
			{
				return f._fun;
			}
		}
		return nullptr;
	}

	CONTECTMENU ProjectTreeConfig::getRootContextMenuFunction(int type, QString des, QString item)
	{
		const int n = _contextFuns.size();
		for (int i = 0; i < n; ++i)
		{
			RegFunction f = _contextFuns.at(i);
			if (f._TreeType == type && f._item == item && f._desibe == des)
			{
				return f._fun;
			}
		}
		return nullptr;
	}

}
