#ifndef DATAMANAGER_H_
#define DATAMANAGER_H_

#include <QObject>
#include <QList>
#include <QHash>

class QDomNodeList;
class QDomNode;
namespace Geant4
{
	class MaterialManager;
	class DefineManager;
	class SolidsManager;
	class StructureManager;
	class PreWindowManager;
	class Geant4DataModel;

	class DataManager : public QObject
	{
		Q_OBJECT
	public:
		static DataManager* getInstance();
		MaterialManager* getMaterialManager();
		DefineManager* getDefineManager();
		StructureManager* getStructManager();
		SolidsManager* getSolidsManager();
		PreWindowManager* getPreWindowManager();
		Geant4DataModel * getGeant4DataModel();

		void clear();
	
		//void readSetup(QDomNodeList& nodeList);
		void readSetup(QDomNode& node);

		void appendSetUp(QString name, QString worldRef);

		QHash<QString, QString> getSetup();

	private:
		DataManager();
		~DataManager();

	private:
		static DataManager* _instance;

		MaterialManager* _materialManager{};
		StructureManager* _structManager{};
		DefineManager* m_defineManager{};
		SolidsManager* m_solidsManager{};
		PreWindowManager* _winManager{};
		Geant4DataModel* _geant4DataModel{};
		QHash<QString, QString>  _setupNameWorldRef{};
	};
}



#endif
