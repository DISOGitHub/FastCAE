#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include <QObject>
#include <QList>
#include <QHash>

namespace Aircraft
{
	/*class AircraftManager;
	class AircraftManager;
	class EngineManager;*/


	class DataManager : public QObject
	{
		Q_OBJECT
	public:
		static DataManager* getInstance();
		/*MaterialManager* getMaterialManager();
		DefineManager* getDefineManager();
		StructureManager* getStructManager();
		SolidsManager* getSolidsManager();
		PreWindowManager* getPreWindowManager();
		Geant4DataModel * getGeant4DataModel();*/

	private:
		DataManager();
		~DataManager();

	private:
		static DataManager* _instance;

		/*MaterialManager* _materialManager{};
		StructureManager* _structManager{};
		DefineManager* m_defineManager{};
		SolidsManager* m_solidsManager{};
		PreWindowManager* _winManager{};
		Geant4DataModel* _geant4DataModel{};
		QHash<QString, QString>  _setupNameWorldRef{};*/
	};
}



#endif
