#include "GDMLReader.h"
#include "DataManager.h"
#include "DefineManager.h"
#include "SolidsManager.h"
#include <QDebug>
#include <QFile>
#include <QDomElement>
#include <QDomDocument>
#include "MaterialManager.h"
#include "StructureManager.h"
#include "settings/busAPI.h"
#include <QFileDialog>
#include <QMessageBox>
namespace Geant4
{
	GDMLReader::GDMLReader(QString filename) : _filename(filename)
	{
	}

	
	bool GDMLReader::read()
	{
		QFile file(_filename);
		if (!file.open(QIODevice::ReadOnly | QFile::Text)) return false;
		auto doc = new QDomDocument;
		int row{ 0 }, col{ 0 };
		QString errstr{};
		if (!doc->setContent(&file, true, &errstr, &row, &col))
		{
			qDebug() << errstr << "line: " << row << "col: " << col;
			file.close();
			return false;
		}
		DataManager* datamanager = Geant4::DataManager::getInstance();

		QDomElement root = doc->documentElement();
		QDomNodeList nodeList = root.childNodes();
		for (int i = 0; i < nodeList.size(); i++)
		{
			QDomNode node = nodeList.at(i);
			if (node.nodeName() == "define")
			{
				datamanager->getDefineManager()->readGDML(node);
			}
			else if (node.nodeName()=="materials")
			{
				datamanager->getMaterialManager()->readGDML(node);
			}
			else if (node.nodeName()=="solids")
			{
				datamanager->getSolidsManager()->readGDML(node);
			}
			else if (node.nodeName()=="structure")
			{
				datamanager->getStructManager()->readGDML(node);
			}
			else if (node.nodeName()=="setup")
			{
				datamanager->readSetup(node);
			}
		}


/*

		QDomNodeList defineList = doc->elementsByTagName("define");
		QDomNodeList maList = doc->elementsByTagName("materials");
		QDomNodeList solidsList = doc->elementsByTagName("solids");
		QDomNodeList structureList = doc->elementsByTagName("structure");
		QDomNodeList setUpList = doc->elementsByTagName("setup");

		datamanager->getDefineManager()->readGDML(defineList);
		datamanager->getMaterialManager()->readGDML(maList);
		datamanager->getSolidsManager()->readGDML(solidsList);
		datamanager->getStructManager()->readGDML(structureList);
		datamanager->readSetup(setUpList);*/
		//bool ss = this->write();
		return true;
	}

	

	bool GDMLReader::write()
	{
		
		QFile file(_filename);
		if (!file.open(QIODevice::WriteOnly | QFile::Truncate))
		{
			QMessageBox::warning(nullptr, "error", "save file failed!");
			return false;
		}

		QDomDocument doc;
		//写入xml头部
		QDomProcessingInstruction instruction; //
		instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
		doc.appendChild(instruction);
		QDomElement root = doc.createElement("gdml");
		root.setAttribute("xmlns:gdml", "http://cern.ch/2001/Schemas/GDML");
		root.setAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
		root.setAttribute("xsi:noNamespaceSchemaLocation", "../GDMLSchema/gdml.xsd");
		

		DataManager* datamanager = Geant4::DataManager::getInstance();
		datamanager->getDefineManager()->writeGDML(doc, root);
		datamanager->getMaterialManager()->writeGDML(doc, root);
		datamanager->getSolidsManager()->writeGDML(doc, root);
		datamanager->getStructManager()->writeGDML(doc, root);

		QHash<QString, QString> setup = datamanager->getSetup();
		QList<QString> skeys = setup.keys();
		for (QString k : skeys)
		{
			QDomElement se = doc.createElement("setup");
			se.setAttribute("name", k);
			se.setAttribute("version", "1");
			QDomElement wele = doc.createElement("world");
			wele.setAttribute("ref", setup.value(k));
			se.appendChild(wele);
			root.appendChild(se);
		}
		
		doc.appendChild(root);
		QTextStream out_stream(&file);
		doc.save(out_stream, 4); //缩进4格
		file.close();
		return true;
	}

}