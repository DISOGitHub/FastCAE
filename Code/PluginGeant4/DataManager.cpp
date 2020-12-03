#include "DataManager.h"
#include "MaterialManager.h"
#include "DefineManager.h"
#include "SolidsManager.h"
#include "StructureManager.h"
#include "PreWindowManager.h"
#include "Geant4Plugin.h"
#include "Geant4DataModel.h"
#include <QDomDocument>
#include <QDebug>

namespace Geant4
{

	DataManager*  DataManager::_instance = nullptr;
	DataManager* DataManager::getInstance()
	{
		if (_instance == nullptr)
			_instance = new DataManager;
		return _instance;
	}


	DataManager::DataManager()
	{
		_materialManager = new MaterialManager;
		m_defineManager = new DefineManager;
		_structManager = new StructureManager;
		m_solidsManager = new SolidsManager;
		_winManager = new PreWindowManager(Geant4Plugin::getMainWindow());
		_geant4DataModel = new Geant4DataModel;
	}

	DataManager::~DataManager()
	{
	}

	Geant4::MaterialManager* DataManager::getMaterialManager()
	{
		return _materialManager;
	}


	DefineManager* DataManager::getDefineManager()
	{
		return m_defineManager;
	}

	SolidsManager* DataManager::getSolidsManager()
	{
		return m_solidsManager;
	}



	PreWindowManager* DataManager::getPreWindowManager()
	{
		return _winManager;
	}

	Geant4DataModel * DataManager::getGeant4DataModel()
	{
		return _geant4DataModel;
	}

	void DataManager::clear()
	{
		_materialManager->clear();
		_structManager->clear();
		m_solidsManager->clear();
		m_defineManager->clear();
		_setupNameWorldRef.clear();
	}

/*
	void DataManager::readSetup(QDomNodeList& nodeList)
	{
		const int n = nodeList.size();
		for (int i = 0; i < n; ++i)
		{
			QString name, ref;
			QDomElement ele = nodeList.at(i).toElement();
			name = ele.attribute("name");

			QDomNodeList wfl = ele.elementsByTagName("world");
			if (wfl.size() != 1) continue;
			ref = wfl.at(0).toElement().attribute("ref");

			qDebug() << "setup:  " << name << "  " << ref;
			this->appendSetUp(name, ref);
		}
	}*/

	void DataManager::readSetup(QDomNode& node)
	{
		QString name, ref;
		QDomElement ele = node.toElement();
		name = ele.attribute("name");

		QDomNodeList wfl = ele.elementsByTagName("world");
		if (wfl.size() != 1) return;
		ref = wfl.at(0).toElement().attribute("ref");

		qDebug() << "setup:  " << name << "  " << ref;
		this->appendSetUp(name, ref);
	}

	StructureManager* DataManager::getStructManager()
	{
		return _structManager;
	}

	void DataManager::appendSetUp(QString name, QString worldRef)
	{
		_setupNameWorldRef.insert(name, worldRef);
	}

	QHash<QString, QString> DataManager::getSetup()
	{
		return _setupNameWorldRef;
	}

}