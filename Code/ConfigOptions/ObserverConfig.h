#ifndef _OBSERVERCONFIG_H_
#define  _OBSERVERCONFIG_H_

#include <QHash>
#include "ConfigOptionsAPI.h"
#include "DataProperty/modelTreeItemType.h"

namespace ConfigOption
{
	class ParameterObserver;

	class CONFIGOPTIONSAPI ObserverConfig
	{
	public:
		ObserverConfig() = default;
		~ObserverConfig() = default;
		
		void clearData();
		void appendObserver(ProjectTreeType type, ParameterObserver* observer);
		QList<ParameterObserver*> getObserverList(ProjectTreeType type);

	private:
		QHash<ProjectTreeType, QList<ParameterObserver*>> _observerList{};
	};


}



#endif


