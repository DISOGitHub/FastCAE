#include "ParameterObserver.h"
#include "DataProperty/ParameterBase.h"
#include "DataProperty/ParameterGroup.h"
#include "DataProperty/ParameterList.h"
#include <QDebug>

namespace ConfigOption
{

	ParameterObserver::~ParameterObserver()
	{
		QList<DataProperty::ParameterBase*> plist = _configActive.values();
		for (int i = 0; i < plist.size(); ++i)
		{
			auto p = plist.at(i);
			delete p;
		}
		_configActive.clear();
		plist = _configfollow.values();
		for (int i = 0; i < plist.size(); ++i)
		{
			auto p = plist.at(i);
			delete p;
		}
		_configfollow.clear();
		QList<DataProperty::ParameterGroup*> gplist = _configfollowGroup.values();
		for (int i = 0; i < gplist.size(); ++i)
		{
			auto p = gplist.at(i);
			delete p;
		}
		_configfollowGroup.clear();
		
		_dataActive.clear();
		_datafollow.clear();
	}

	void ParameterObserver::appendCofigActive(QString name, DataProperty::ParameterBase* p)
	{
		if (_configActive.contains(name))
		{
			DataProperty::ParameterBase* o = _configActive.value(name);
			delete o;
		}
		_configActive[name] = p;
	}

	void ParameterObserver::appendConfigFollow(QString name, DataProperty::ParameterBase* p)
	{
		if (_configfollow.contains(name))
		{
			DataProperty::ParameterBase* o = _configfollow.value(name);
			delete o;
		}
		_configfollow[name] = p;
	}

	void ParameterObserver::appendConfigFollowGroup(QString name, DataProperty::ParameterGroup* p)
	{
		if (_configfollowGroup.contains(name))
		{
			auto g = _configfollowGroup.value(name);
			delete g;
		}
		_configfollowGroup[name] = p;
	}

	ParameterObserver* ParameterObserver::copy()
	{
		ParameterObserver* observer = new ParameterObserver;
		QList<QString> names = _configActive.keys();
		for (int i = 0; i < names.size(); ++i)
		{
			QString name = names.at(i);
			auto p = _configActive.value(name);
			auto cp = DataProperty::ParameterList::copyParameter(p);
			if (cp != nullptr)
				observer->appendCofigActive(name,cp);
		}
		names = _configfollow.keys();
		for (int i = 0; i < names.size(); ++i)
		{
			QString name = names.at(i);
			auto p = _configfollow.value(name);
			auto cp = DataProperty::ParameterList::copyParameter(p);
			if (cp != nullptr)
				observer->appendConfigFollow(name, cp);
		}
		names = _configfollowGroup.keys();
		for (int i = 0; i < names.size(); ++i)
		{
			QString name = names.at(i);
			auto g = _configfollowGroup.value(name);
			if (g == nullptr) continue;
			DataProperty::ParameterGroup* cg = new DataProperty::ParameterGroup;
			cg->copy(g);
			observer->appendConfigFollowGroup(name, cg);
		}
		return observer;
	}

	void ParameterObserver::appendDataActive(QString name, DataProperty::ParameterBase*p)
	{
		_dataActive[name] = p;
		connect(p, SIGNAL(dataChanged()), this, SLOT(observe()));
	}

	void ParameterObserver::appendDataFollow(QString name, DataProperty::ParameterBase*p)
	{
		_datafollow[name] = p;
	}

	void ParameterObserver::appendDataFollowGroup(QString name, DataProperty::ParameterGroup* p)
	{
		_datafollowGroup[name] = p;
	}


	QStringList ParameterObserver::getActiveParameterNames()
	{
		return _configActive.keys();
	}

	QStringList ParameterObserver::getFollowParameterNames()
	{
		return _configfollow.keys();
	}

	QStringList ParameterObserver::getFollowGroupNames()
	{
//		qDebug() << _configfollowGroup.keys();
		return _configfollowGroup.keys();
	}

	void ParameterObserver::removeConfigActive(QString name)
	{
		auto para = _configActive.value(name);
		if (para != nullptr)
		{
			delete para;
			_configActive.remove(name);
		}	
	}
	void ParameterObserver::removeConfigFollow(QString name)
	{
		auto para = _configfollow.value(name);
		if (para != nullptr)
		{
			delete para;
			_configfollow.remove(name);

		}
	}

	void ParameterObserver::removeConfigFollowGroup(QString name)
	{
		auto pg = _configfollowGroup.value(name);
		if (pg != nullptr)
		{
			delete pg;
			_configfollowGroup.remove(name);
		}
	}

	void ParameterObserver::observe()
	{
		if (check())
		{
			changeStates();
			emit updateDisplay();
		}
			
	}

	bool ParameterObserver::check()
	{
		bool same = true;
		QStringList activenames = _configActive.keys();
		for (int i = 0; i < activenames.size(); ++i)
		{
			QString name = activenames.at(i);
			auto cp = _configActive.value(name);
			auto dp = _dataActive.value(name);
			if ((cp == nullptr) || (dp == nullptr)) continue;
			if (!cp->isSameValueWith(dp))
			{
				same = false;
				break;
			}
		}
		return same;
	}

	void ParameterObserver::changeStates()
	{
		QStringList follolwnames = _configfollow.keys();
		for (int i = 0; i < follolwnames.size(); ++i)
		{
			QString name = follolwnames.at(i);
			auto cp = _configfollow.value(name);
			auto dp = _datafollow.value(name);
			if ((cp == nullptr) || (dp == nullptr)) continue;
			dp->copy(cp, true);
			dp->copyStatus(cp);
		}
		follolwnames = _configfollowGroup.keys();
		for (int i = 0; i < follolwnames.size(); ++i)
		{
			QString name = follolwnames.at(i);
			auto cg = _configfollowGroup.value(name);
			auto dg = _datafollowGroup.value(name);
			if ((cg == nullptr) || (dg == nullptr)) continue;
			dg->copyStates(cg);
		}

	} 



	

}