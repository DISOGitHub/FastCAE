#ifndef MATERIALMANAGER_H_
#define MATERIALMANAGER_H_

#include <QObject>

class QDomNodeList;
class QDomNode;
class QDomDocument;
class QDomElement;
namespace Geant4
{
	class GDMLElement;
	class GDMLMaterial;
	class GDMLIsotope;

	class MaterialManager : public QObject
	{
		Q_OBJECT
	public:
		MaterialManager() = default;
		~MaterialManager() = default;

		void appendElement(GDMLElement* ele);
		void appendMaterial(GDMLMaterial* mat);
		void appendIsotope(GDMLIsotope* iso);

		int getElementCount();
		int getMaterialCount();
		int getIsotopeCount();

		QList<GDMLElement*> getElementList();
		GDMLElement* getElementByIndex(int index);

		QList <GDMLMaterial*> getMaterialList();
		GDMLMaterial* getMaterialByIndex(int index);

		QList <GDMLIsotope*> getIsotopeList();
		GDMLIsotope* getIsotopeByIndex(int index);

		//void readGDML(QDomNodeList &nodes);
		void readGDML(QDomNode &node);
		void writeGDML(QDomDocument& doc, QDomElement& root);

		void writeGDMLIsotope(QDomDocument& doc, QDomElement & materials);
		void writeGDMLElement(QDomDocument& doc, QDomElement & materials);
		void writeGDMLMaterial(QDomDocument& doc, QDomElement & materials);
		void clear();

	private:
		
		QList<GDMLIsotope*> _isotopelist{};
		QList<GDMLElement*> _elelist{};
		QList<GDMLMaterial*> _matlist{};
	};
}



#endif
