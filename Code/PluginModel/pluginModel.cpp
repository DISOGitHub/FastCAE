#include "pluginModel.h"
//#include "mainWindow/mainWindow.h"
//#include "mainWindow/SubWindowManager.h"
//#include "model.h"
//#include "tree.h"
//#include <QString>
//#include "ModelData/modelDataFactory.h"
//#include "ModelData/modelDataSingleton.h"
//#include "ModelData/modelDataBase.h"
//#include "MainWidgets/ProjectTreeFactory.h"
#include "IO/IOConfig.h"
//#include "Material/MaterialFactory.h"
//#include "Material/Material.h"
#include "MReader.h"
#include <QDebug>
#include "meshData/meshSet.h"
#include "meshData/meshKernal.h"
//#include <vtkPoints.h>
#include <vtkCell.h>
//#include <vtkIdList.h>
//#include <QTextStream>
#include <QFileDialog>
#include <qdom.h>
//#include "settings/busAPI.h"
//#include "BCBase/BCVectorBase.h"
#include "BCBase/BCDisplacement.h"
#include "ModelData/ModelDataBaseExtend.h"
#include "geometry/GeoComponent.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
//#include "python/PyAgent.h"
#include "Gmsh/GmshThread.h"
#include "Gmsh/GmshModule.h"
#include "moduleBase/processBar.h"
#include "Gmsh/GmshThreadManager.h"
#include "BCBase/BCPressure.h"
#include "BCBase/BCTemperature.h"

GUI::MainWindow* Plugins::ModelPlugin::_mainwindow = nullptr;

void Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* ps)
{
	Plugins::PluginBase* p = new Plugins::ModelPlugin(m);
	ps->append(p);
}

namespace Plugins
{
	ModelPlugin::ModelPlugin(GUI::MainWindow* m)
	{
		_mainwindow = m;
		_describe = "model Test plugin";
	}

	ModelPlugin::~ModelPlugin()
	{

	}

	bool ModelPlugin::install()
	{
// 		ModelData::ModelDataFactory::registerType(PluginDefType+1, "PluginType0001", CreateModel);
// 		MainWidget::ProjectTreeFactory::registerType(PluginDefType + 1, CreateTree);
// 
 		IO::IOConfigure::RegisterInputFile("out", WriteOut);
// 		IO::IOConfigure::RegisterOutputTransfer("trans", transfer);
//		IO::IOConfigure::RegisterMeshImporter("m", importMeshFun);
// 		IO::IOConfigure::RegisterMeshExporter("iii", exportMeshFun);
// 
// 		Material::MaterialFactory::registerType("ma1", createMaterial);
		return true;
	}

	bool ModelPlugin::uninstall()
	{
// 		ModelData::ModelDataFactory::removeType(PluginDefType + 1);
// 		MainWidget::ProjectTreeFactory::removeType(PluginDefType + 1);
// 
// 		IO::IOConfigure::RemoveInputFile("out");
// 		IO::IOConfigure::RemoveOutputTransfer("trans");
		IO::IOConfigure::RemoveMeshImporter("m");
// 		IO::IOConfigure::RemoveMeshExporter("iii");
 
// 		Material::MaterialFactory::removeType("ma1");
		return true;
	}

	GUI::MainWindow* ModelPlugin::getMWpt()
	{
		return _mainwindow;
	}
}

bool MODELPLUGINAPI importMeshFun(QString file)
{
	Plugins::MReader reader(file);
	bool ok = reader.read();
	return ok;
}

// bool CreateModel(int t, QPair<int, ModelData::ModelDataBase*>* p)
// {
// 	if (t == ProjectTreeType::PluginDefType + 1)
// 	{
// 		auto m = new ModelData::ModelDataPlugin((ProjectTreeType)t);
// 		p->first = t;
// 		p->second = m;
// 		return true;
// 	}
// 	return false;
// 		
// 
// }

// bool CreateTree(int tp, GUI::MainWindow* m, QPair<int, ProjectTree::ProjectTreeBase*>* t)
// {
// 	if (tp == PluginDefType + 1)
// 	{
// 		t->first = tp;
// 		t->second = new ProjectTree::PluginTree(m);
// 		return true;
// 	}
// 	return false;
// }


bool WriteOut(QString path, ModelData::ModelDataBase* d)
{
	ModelData::ModelDataBaseExtend* modelData = static_cast<ModelData::ModelDataBaseExtend*>(d);
	if (!modelData)    return false;
	QList<int> IDs = modelData->getComponentIDList();
	if (IDs.size() < 1)    return false;

	QDomDocument doc;
	QDomProcessingInstruction xmlInstruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
	doc.appendChild(xmlInstruction);

	QDomElement root = doc.createElement("ModelData");
	root.setAttribute("ModelName", modelData->getName());
	root.setAttribute("ModelID", modelData->getID());
	doc.appendChild(root);

	QDomElement BCsEle = doc.createElement("BC");
	BCsEle.setAttribute("BCCount", modelData->getBCCount());
	root.appendChild(BCsEle);
	WriteOutBC(&doc, &BCsEle, path, modelData);

	QDomElement componentsEle = doc.createElement("Components");
	componentsEle.setAttribute("ComponentCount", IDs.size());
	root.appendChild(componentsEle);

	MeshData::MeshData* meshdata = MeshData::MeshData::getInstance();
	MeshData::MeshSet* ms = meshdata->getMeshSetByID(IDs[0]);
	if (ms)
		WriteOutMeshSet(&doc, &componentsEle, IDs, path);
	else
		WriteOutGeoComponent(&doc, &componentsEle, IDs, path);
	return false;
}

void MODELPLUGINAPI WriteOutMeshSet(QDomDocument* doc, QDomElement* parent, QList<int>& msIDs, QString& path)
{
	if (msIDs.size() == 0)	return;
	MeshData::MeshData* meshData = MeshData::MeshData::getInstance();
	for (int msID : msIDs)
	{
		MeshData::MeshSet* meshSet = meshData->getMeshSetByID(msID);
		if (!meshSet)	continue;
		vtkDataSet* data = meshSet->getDisplayDataSet();

		QDomElement aComponentEle = doc->createElement("aComponent");
		aComponentEle.setAttribute("ComponentID", msID);
		aComponentEle.setAttribute("ComponentName", meshSet->getName());
		aComponentEle.setAttribute("ComponentSource", "Mesh");
		parent->appendChild(aComponentEle);

		int pointsCount = data->GetNumberOfPoints();
		QDomElement pointsEle = doc->createElement("Points");
		pointsEle.setAttribute("Points Count", data->GetNumberOfPoints());
		aComponentEle.appendChild(pointsEle);
		for (int i = 0; i < pointsCount; i++)
		{
			double* array = data->GetPoint(i);
			QString pointStr = QString("%1,   %2,   %3").arg(array[0]).arg(array[1]).arg(array[2]);
			QDomText pointText = doc->createTextNode(pointStr);
			QDomElement pointEle = doc->createElement("Point");
			pointsEle.appendChild(pointEle);
			pointEle.appendChild(pointText);
		}

		int cellsCount = data->GetNumberOfCells();
		QDomElement cellsEle = doc->createElement("Cells");
		cellsEle.setAttribute("Cells Count", data->GetNumberOfCells());
		aComponentEle.appendChild(cellsEle);
		for (int i = 0; i < cellsCount; i++)
		{
			vtkCell* cell = data->GetCell(i);
			if (cell == nullptr) continue;
			vtkIdList* idIndexs = cell->GetPointIds();
			int idCount = idIndexs->GetNumberOfIds();
			QString cellStr;
			for (int j = 0; j <= idCount - 1; j++)
				cellStr += QString::number(idIndexs->GetId(j)) + "   ";
			QDomText cellText = doc->createTextNode(cellStr.left(cellStr.size() - 3));
			QDomElement cellEle = doc->createElement("Cell");
			cellsEle.appendChild(cellEle);
			cellEle.appendChild(cellText);
		}
	}

	QString strFile = path + "/test.xml";
	QFile file(strFile);
	if (file.open(QFile::WriteOnly | QFile::Text))
	{
		QTextStream out(&file);
		doc->save(out, QDomNode::EncodingFromDocument);
		file.close();
	}
}

void MODELPLUGINAPI WriteOutGeoComponent(QDomDocument* doc, QDomElement* parent, QList<int>& gcIDs, QString& path)
{
	if (gcIDs.size() == 0)	return;
	Geometry::GeometryData* geoData = Geometry::GeometryData::getInstance();
	QMultiHash<int, int> Surface, Body;
	for (int gcID : gcIDs)
	{
		Geometry::GeoComponent* gc = geoData->getGeoComponentByID(gcID);
		if (!gc)	continue;
		QMultiHash<Geometry::GeometrySet*, int> items = gc->getSelectedItems();
		QMultiHash<Geometry::GeometrySet*, int>::const_iterator cit;
		for (cit = items.constBegin(); cit != items.constEnd(); cit++)
		{
			int ID = cit.key()->getID();
			int index = cit.value();
			if (gc->getGCType() == Geometry::GeoComponentType::Surface)
				Surface.insert(ID, index);
			else if (gc->getGCType() == Geometry::GeoComponentType::Body)
				Body.insert(ID, index);
		}
	}
	GenerateMesh(doc, parent, Surface, Body, path);
}

void MODELPLUGINAPI GenerateMesh(QDomDocument* doc, QDomElement* parent, QMultiHash<int, int>& Surface, QMultiHash<int, int>& Body, QString& path)
{
	GUI::MainWindow* mw = Plugins::ModelPlugin::getMWpt();
	if (!mw)	return;
	Gmsh::GMshPara* para = new Gmsh::GMshPara;
	para->_solidHash = Body;
	para->_surfaceHash = Surface;
	para->_elementType = "Tet";
	para->_method = 1;
	para->_dim = 3;
	para->_elementOrder = 1;
	para->_sizeFactor = 1.00;
	para->_maxSize = 100.00;
	para->_minSize = 0.00;
	para->_isGridCoplanar = true;
	para->_geoclean = true;

	Gmsh::GmshModule* gModule = Gmsh::GmshModule::getInstance(mw);
	Gmsh::GmshThread* thread = gModule->iniGmshThread(para);
	thread->isSaveDataToKernal(false);
	auto processBar = new ModuleBase::ProcessBar(mw, QObject::tr("Gmsh Working..."));
	Gmsh::GmshThreadManager* manager = gModule->getGmshThreadManager();
	manager->insertThread(processBar, thread);
}

void MODELPLUGINAPI WriteOutBC(QDomDocument* doc, QDomElement* parent, QString& path, ModelData::ModelDataBaseExtend* model)
{
	int BCCount = model->getBCCount();
	for (int i = 0; i < BCCount; i++)
	{
		BCBase::BCBase* bc = model->getBCAt(i);
		QDomElement aBCEle = doc->createElement("aBC");
		parent->appendChild(aBCEle);
		BCBase::BCType bcType = bc->getBCType();
		QString strBCType = BCBase::BCTypeToString(bcType);
		aBCEle.setAttribute("BCID", bc->getID());
		aBCEle.setAttribute("BCType", strBCType);
		aBCEle.setAttribute("BCType", bc->getComponentID());

		switch (bcType)
		{
		case BCBase::BCType::Pressure:
		{
			BCBase::BCPressure* preBC = dynamic_cast<BCBase::BCPressure*> (bc);
			preBC->writeToProjectFile(doc, &aBCEle);
			break;
		}
		case BCBase::BCType::Velocity:
		{
			BCBase::BCVectorBase* velBC = dynamic_cast<BCBase::BCVectorBase*> (bc);
			velBC->writeToProjectFile(doc, &aBCEle);
			break;
		}
		case BCBase::BCType::Temperature:
		{
			BCBase::BCTemperature* temBC = dynamic_cast<BCBase::BCTemperature*> (bc);
			temBC->writeToProjectFile(doc, &aBCEle);
			break;
		}
		case BCBase::BCType::FixSupport:
		{
			bc->writeToProjectFile(doc, &aBCEle);
			break;
		}
		case BCBase::BCType::Displacement:
		{
			BCBase::BCDisplacement* disBC = dynamic_cast<BCBase::BCDisplacement*> (bc);
			disBC->writeToProjectFile(doc, &aBCEle);
			break;
		}
		case BCBase::BCType::AngleVelocity:
		{
			BCBase::BCScalarBase* scaBC = dynamic_cast<BCBase::BCScalarBase*> (bc);
			scaBC->writeToProjectFile(doc, &aBCEle);
			break;
		}
		case BCBase::BCType::Rotation:
		{
			BCBase::BCScalarBase* rotBC = dynamic_cast<BCBase::BCScalarBase*> (bc);
			rotBC->writeToProjectFile(doc, &aBCEle);
			break;
		}
		case BCBase::BCType::Acceleration:
		{
			BCBase::BCVectorBase* vecBC = dynamic_cast<BCBase::BCVectorBase*> (bc);
			vecBC->writeToProjectFile(doc, &aBCEle);
			break;
		}
		case BCBase::BCType::AngleAcceleration:
		{
			BCBase::BCScalarBase* angBC = dynamic_cast<BCBase::BCScalarBase*> (bc);
			angBC->writeToProjectFile(doc, &aBCEle);
			break;
		}
		case BCBase::BCType::Inlet:
		{
			bc->writeToProjectFile(doc, &aBCEle);
			break;
		}
		case BCBase::BCType::Outlet:
		{
			bc->writeToProjectFile(doc, &aBCEle);
			break;
		}
		case BCBase::BCType::Symmetry:
		{
			bc->writeToProjectFile(doc, &aBCEle);
			break;
		}
		case BCBase::BCType::Wall:
		{
			bc->writeToProjectFile(doc, &aBCEle);
			break;
		}
		}
	}
}

// bool transfer(QString path)
// {
// 	qDebug() << path;
// 	qDebug() << "transfer";
// 	return true;
// }

// bool MODELPLUGINAPI exportMeshFun(QString, int)
// {
// 	qDebug() << "meshExport";
// 	return true;
// }
// 
// Material::Material* createMaterial(QString t)
// {
// 	if (t != "ma1") return nullptr;
// 
// 	auto m = new Material::Material;
// 	
// 	auto p = m->appendParameter(DataProperty::Para_Double);
// 	p->setDescribe("ma Name");
// 	return m;
// }
