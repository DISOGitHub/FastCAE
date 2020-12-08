#include "ProjectFileIO.h"
#include <QDomDocument>
#include <QTextStream>
#include "geometry/geometryData.h"
#include "meshData/meshSingleton.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"
#include "ModelData/modelDataBaseExtend.h"
#include "ModelData/modelDataFactory.h"
#include "DataProperty/modelTreeItemType.h"
#include "Material/MaterialSingletion.h"
#include "geometry/geometrySet.h"
#include "moduleBase/CommonFunctions.h"
#include "PluginManager/PluginManager.h"
#include "mainWindow/signalHandler.h"
#include <QDebug>
#include <QDomNodeList>
#include <QDomElement>
#include <QDomText>
#include <QDomAttr>
#include <QCoreApplication>
#include <QDir>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <TopoDS_Shape.hxx>
#include <JlCompress.h>

namespace IO
{
	ProjectFileIO::ProjectFileIO(GUI::MainWindow* mw, GUI::SignalHandler*sh, const QString& fileName, bool read)
		:IOBase(fileName), ModuleBase::ThreadTask(mw), _read(read)
	{
		_geoData = Geometry::GeometryData::getInstance();
		_meshData = MeshData::MeshData::getInstance();
		_modelData = ModelData::ModelDataSingleton::getinstance();
		_materialData = Material::MaterialSingleton::getInstance();
		_plugins = Plugins::PluginManager::getInstance();

		connect(this, SIGNAL(processFinished(QString, bool, bool)), sh, SIGNAL(projectFileProcessedSig(QString, bool, bool)));
	}

	void ProjectFileIO::run()
	{
		bool success = false;
		if (_read)
		{
			emit showInformation(tr("Opening Project File: \"%1\"").arg(_filename));
			success = this->read();
		}
		else
		{
			emit showInformation(tr("Saving Project File: \"%1\"").arg(_filename));
			success = this->write();
		}
		emit processFinished(_filename, success, _read);
		ThreadTask::threadTaskFinished();
	}

	bool ProjectFileIO::write(QString s)
	{
		if (s.isEmpty()) s = _filename;

		bool ok = false;
		QString suffix = s.right(4).toLower();
		qDebug() << s << suffix;
		if (suffix == ".xml")
			ok = writeXml(s);
		else if (suffix == "diso")
			ok = writeDiso(s);

		return ok;
	}

	bool ProjectFileIO::writeDiso(QString fileName)
	{
 		bool ok = false;
		QString exelPath = QCoreApplication::applicationDirPath();
		const QString tempPath = exelPath + "/../tempIO/";

		bool okremove = RemoveDir(tempPath);

		QDir dir = QDir(tempPath);
		if (!dir.exists())
			dir.mkdir(tempPath);

		bool isEmpty = true;
		_doc = new QDomDocument;
		QDomProcessingInstruction instruction = _doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		_doc->appendChild(instruction);
		QDomElement root = _doc->createElement("DISO_FILE_1.0");
		_doc->appendChild(root);
	
		const int geoCount = _geoData->getGeometrySetCount();
		if (geoCount > 0)  //写出几何
		{
			isEmpty = false;
			_geoData->writeToProjectFile(_doc, &root, true);
		}
 		const int meshCount = _meshData->getKernalCount();
 		if (meshCount > 0) //写出网格
 		{
 			isEmpty = false;
// 			_meshData->writeToProjectFile(_doc, &root);
 		}
		const int nMaterial = _materialData->getMaterialCount();
		if (nMaterial > 0)  //写出材料
		{
			isEmpty = false;
			_materialData->writeToProjectFile(_doc, &root);
		}

		const int modelCount = _modelData->getModelCount();
		if (modelCount > 0) //写出物理模型
		{
			isEmpty = false;
			_modelData->writeToProjectFile(_doc, &root);
		}

		_plugins->writeToProjectFile(_doc, &root);
//		if (isEmpty) return false;  //数据为空 不写文件
		QFile file(tempPath + "case.xml");
		if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) return false;
		_stream = new QTextStream(&file);
		_doc->save(*_stream, 4);
		file.close();
		//写网格
	
		if (meshCount > 0)
		{
			QFile mhFile(tempPath + "mesh.mh");
			if (!mhFile.open(QIODevice::WriteOnly)) return false;
			_dataStream = new QDataStream(&mhFile);
			_meshData->writeBinaryFile(_dataStream);			
			mhFile.close();
		}

		ok = JlCompress::compressDir(fileName, tempPath);
		return ok;
	}

	bool ProjectFileIO::writeXml(QString fileName)
	{
		bool isEmpty = true;
		_doc = new QDomDocument;
		QDomProcessingInstruction instruction = _doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		_doc->appendChild(instruction);
		QDomElement root = _doc->createElement("DISO_FILE_1.0");
		_doc->appendChild(root);
		const int geoCount = _geoData->getGeometrySetCount();
		if (geoCount > 0)  //写出几何
		{
			isEmpty = false;
			_geoData->writeToProjectFile(_doc, &root);
		}
		const int meshCount = _meshData->getKernalCount();
		if (meshCount > 0) //写出网格
		{
			isEmpty = false;
			_meshData->writeToProjectFile(_doc, &root);
		}
		const int nMaterial = _materialData->getMaterialCount();
		if (nMaterial > 0)  //写出材料
		{
			isEmpty = false;
			_materialData->writeToProjectFile(_doc, &root);
		}

		const int modelCount = _modelData->getModelCount();
		if (modelCount > 0) //写出物理模型
		{
			isEmpty = false;
			_modelData->writeToProjectFile(_doc, &root);
		}
		_plugins->writeToProjectFile(_doc, &root);
//		if (isEmpty) return false;  //数据为空 不写文件
		if (!_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) return false;
		_stream = new QTextStream(&_file);
		_doc->save(*_stream, 4);
		_file.close();

		return true;
	}

	bool ProjectFileIO::read()
	{
		bool ok = false;
		QString f = _filename;
		//qDebug() << f;
		QString suffix = f.right(4);
		if (suffix == ".xml")
			ok = readXml(f);
		else if (suffix == "diso")
			ok = readDiso(f);
		return ok;
	}

	bool ProjectFileIO::readXml(QString fileName)
	{
		if (!_file.open(QIODevice::ReadOnly | QFile::Text)) return false;
		_doc = new QDomDocument;
		int row{0}, col{0};
		QString errstr{};
		if (!_doc->setContent(&_file,true,&errstr,&row,&col))
		{
			qDebug() << errstr << "line: " << row << "col: " << col;
			_file.close();
			return false;
		}
		QDomNodeList modelNodeList = _doc->elementsByTagName("ModelData");
		QDomNodeList geoNodeList = _doc->elementsByTagName("Geometry");
		QDomNodeList meshNodeList = _doc->elementsByTagName("Mesh");
		QDomNodeList materialList = _doc->elementsByTagName("Materials");
		QDomNodeList pluginList = _doc->elementsByTagName("Plugins");
// 		if (modelNodeList.size() + geoNodeList.size() + meshNodeList.size() <= 0)
// 		{
// 			_file.close();
// 			return false;
// 		}
		readGeoData(&geoNodeList);
		readMeshData(&meshNodeList);
		readMaterialData(&materialList);
		readModelData(&modelNodeList);
		readPluginData(&pluginList);
		_file.close();

		return true;
	}

	bool ProjectFileIO::readDiso(QString fileName)
	{
		QString exelPath = QCoreApplication::applicationDirPath();
		const QString tempPath = exelPath + "/../tempIO/";

		bool okremove = RemoveDir(tempPath);
		if (okremove)
		{
			QDir dir = QDir(tempPath);
			if (!dir.exists())
				dir.mkdir(tempPath);
		}

		QStringList files = JlCompress::extractDir(fileName, tempPath);
		if (files.size() == 0) return false;

		QFile file(tempPath + "case.xml");
		if (!file.open(QIODevice::ReadOnly | QFile::Text)) return false;
		_doc = new QDomDocument;
		if (!_doc->setContent(&file))
		{
			file.close();
			return false;
		}
		QDomNodeList modelNodeList = _doc->elementsByTagName("ModelData");
		QDomNodeList geoNodeList = _doc->elementsByTagName("Geometry");
//		QDomNodeList meshNodeList = _doc->elementsByTagName("Mesh");
		QDomNodeList materialList = _doc->elementsByTagName("Materials");
		QDomNodeList pluginList = _doc->elementsByTagName("Plugins");
// 		if (modelNodeList.size() + geoNodeList.size() /*+ meshNodeList.size()*/ <= 0)
// 		{
// 			_file.close();
// //			return false;
// 		}
		readGeoData(&geoNodeList,true);
//		readMeshData(&meshNodeList);

		QFile mhFile(tempPath + "mesh.mh");
		if (mhFile.open(QIODevice::ReadOnly))
		{
			_dataStream = new QDataStream(&mhFile);
			_meshData->readBinaryFile(_dataStream);
			_meshData->generateDisplayDataSet();
			mhFile.close();
		}

		readMaterialData(&materialList);
		readModelData(&modelNodeList);
		readPluginData(&pluginList);
		file.close();

		return true;
	}

	void ProjectFileIO::readGeoData(QDomNodeList* nodeList,bool isdiso)
	{
		const int nodeCount = nodeList->size();
		if (nodeCount != 1) return;
		_geoData->readFromProjectFile(nodeList,isdiso);
	}
	void ProjectFileIO::readMeshData(QDomNodeList* nodeList)
	{
		const int nodeCount = nodeList->size();
		if (nodeCount != 1) return;
		_meshData->readFromProjectFile(nodeList);
		_meshData->generateDisplayDataSet();
	}
	void ProjectFileIO::readMaterialData(QDomNodeList* nodelist)
	{
		const int nodeCount = nodelist->size();
		if (nodeCount != 1) return;
		QDomElement ele = nodelist->at(0).toElement();
		_materialData->readDataFromProjectFile(&ele);
	}

	void ProjectFileIO::readPluginData(QDomNodeList* nodeList)
	{
		const int nodeCount = nodeList->size();
		if (nodeCount != 1) return;
		QDomElement ele = nodeList->at(0).toElement();
		_plugins->readDataFromProjectFile(&ele);
	}

	void ProjectFileIO::readModelData(QDomNodeList* nodeList)
	{
		const int nodeCount = nodeList->size();
		if (nodeCount != 1) return;
		QDomElement root = nodeList->at(0).toElement();
		QDomNodeList modelList = root.elementsByTagName("Model");
		const int modelCount = modelList.size();
		for (int i = 0; i < modelCount; ++i)
		{
			QDomElement model = modelList.at(i).toElement();
			QString stype = model.attribute("Type");
			ProjectTreeType type = getTreeTypeByString(stype);
			ModelData::ModelDataBase *modelData = nullptr;
			modelData = ModelData::ModelDataFactory::createModel(type);
			if (modelData == nullptr) continue;

			modelData->setID(model.attribute("ID").toInt());
			modelData->setName(model.attribute("Name"));
			modelData->readDataFromProjectFile(&model);
			modelData->generateParaInfo();
			_modelData->appendModel(modelData);
		}

	}



}