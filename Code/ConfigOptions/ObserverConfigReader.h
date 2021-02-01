#ifndef _OBSERVERCONFIGREADER_H_
#define _OBSERVERCONFIGREADER_H_

#include <QString>
#include "DataProperty/modelTreeItemType.h"

class QDomElement;

namespace ConfigOption
{
	class ObserverConfig;

	class ObserverConfigReader
	{
	public:
		ObserverConfigReader(QString file, ObserverConfig* oc);
		~ObserverConfigReader() = default;

		bool read();

	private:
		void readObserverList(QDomElement* ele, ProjectTreeType type);
		void readObserver(QDomElement* ele, ProjectTreeType type);

	private:
		ObserverConfig* _observerConfig{};
		QString _filename{};
	};

	
}


#endif
