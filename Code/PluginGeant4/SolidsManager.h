#ifndef _SOLIDSMANAGER_H
#define _SOLIDSMANAGER_H

#include <QList>

class QDomNodeList;
class QDomNode;
class QDomDocument;
class QDomElement;

namespace Geant4 
{
	class SolidShapeBase;

	class SolidsManager
	{
	public:
		SolidsManager() = default;
		~SolidsManager() = default;

		void appendSolid(SolidShapeBase* s);
		int getSolidCount();
		SolidShapeBase* getSolidAt(int index);
		SolidShapeBase* getSolidByName(QString name);
		void deleteSolidByName(QString name);
		bool isSolid(QString name);

		void clear();

		//bool readGDML(QDomNodeList & nodes);
		bool readGDML(QDomNode & node);
		void writeGDML(QDomDocument& doc, QDomElement& ele);
	
	private:
		QList<SolidShapeBase*> m_geometry_list{};
		
	};
}
#endif // _SOLIDSMANAGER_H


