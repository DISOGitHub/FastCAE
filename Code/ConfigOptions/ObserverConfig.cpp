#include "ObserverConfig.h"
#include "ParameterObserver.h"

namespace ConfigOption
{
	void ObserverConfig::clearData()
	{
		QList<ProjectTreeType> typelist = _observerList.keys();
		for (int i = 0; i < typelist.size(); ++i)
		{
			ProjectTreeType type = typelist.at(i);
			QList<ParameterObserver*> observerlist = _observerList.value(type);
			for (int j = 0; j < observerlist.size(); ++j)
			{
				ParameterObserver* p = observerlist.at(j);
				delete p;
			}
		}
		_observerList.clear();
	}

	void ObserverConfig::appendObserver(ProjectTreeType type, ParameterObserver* observer)
	{
		if (observer == nullptr) return;
		_observerList[type].append(observer);
	}

	QList<ParameterObserver*> ObserverConfig::getObserverList(ProjectTreeType t)
	{
		return _observerList.value(t);
	}

}