#ifndef _DEFINEMANAGER_H
#define _DEFINEMANAGER_H

#include <QList>

class QDomDocument;
class QDomElement;
class QDomNodeList;
class QDomNode;

namespace Geant4
{
	class PreDefineBase;

	class DefineManager
	{
	public:
		DefineManager() = default;
		~DefineManager() = default;

		int getDefineCount();

		PreDefineBase* getPreDefineAt(int index);
		PreDefineBase* getPreDefineByName(QString name);

		void clear();

		
		bool readGDML(QDomNodeList & nodes);
		bool readGDML(QDomNode & node);

		void writeGDML(QDomDocument& doc, QDomElement& ele);

	private:
		QList<PreDefineBase*> _defineList{};
		

	};
}

#endif // _DEFINEMANAGER_H


