#ifndef _STRUCTUREMANAGER_H_
#define _STRUCTUREMANAGER_H_

#include <QList>

class QDomNodeList;
class QDomNode;
class QDomDocument;
class QDomElement;

namespace Geant4
{
	class Volume;
	class Assembly;
	class Physvol;

	class StructureManager
	{
	public:
		StructureManager() = default;
		~StructureManager() = default;

		void appenValume(Volume* v);
		void appendAssembly(Assembly* a);

		//0-solid 1-volume 2-assembly
		int getType(QString name);

		int getVolumeCount();
		Volume* getVolumeAt(int index);
		Volume* getVolumeByName(QString name);

		Physvol* getPhysvolAt(int vindex, int pIndex);
		
		QList<Physvol*> getAllPhysvol();

		//bool readGDML(QDomNodeList & nodes);
		bool readGDML(QDomNode & node);
		void writeGDML(QDomDocument& doc, QDomElement& ele);

		void clear();

	private:
		QList<Volume*> _volumeList{};
		QList<Assembly*> _assemblyList{};
	};


}



#endif
