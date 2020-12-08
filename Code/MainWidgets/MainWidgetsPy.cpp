#include "MainWidgetsPy.h"
#include "ControlPanel.h"
#include "GeometryWidget.h"
#include "MeshWidget.h"
#include "PhysicsWidget.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/ProjectTreeConfig.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBaseExtend.h"
#include "ModelData/modelDataFactory.h"
#include "ProjectTree/ProjectTreeWithBasicNode.h"
#include "Material/MaterialSingletion.h"
#include "python/PyAgent.h"
#include <QDebug>
#include <assert.h>
#include "mainWindow/mainWindow.h"
#include "Material/MaterialSingletion.h"
#include "Material/Material.h"
#include <QMessageBox>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QFile>
#include <QApplication>
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/MaterialConfig.h"
#include "Material/MaterialFactory.h"

namespace MainWidget
{

	GUI::MainWindow* MainWidgetPy::_mainWindow = nullptr;
	ControlPanel* MainWidgetPy::_controlPanel = nullptr;
	GeometryTreeWidget* MainWidgetPy::_geoWidget = nullptr;
	MeshWidget* MainWidgetPy::_meshWidget = nullptr;
	PhysicsWidget* MainWidgetPy::_physicsWidget = nullptr;
	Py::PythonAagent* MainWidgetPy::_pyAgent = nullptr;

	void MainWidgetPy::init(GUI::MainWindow* mw, ControlPanel* p, GeometryTreeWidget*g, MeshWidget* m, PhysicsWidget* ph)
	{
		_mainWindow = mw;
		_controlPanel = p;
		_geoWidget = g;
		_meshWidget = m;
		_physicsWidget = ph; 
		_pyAgent = Py::PythonAagent::getInstance();
		
	}
	void MainWidgetPy::createCase(char* cname, char* ctype)
	{
		QString name = QString(cname);
		QString type = QString(ctype);
	
		ModelData::ModelDataBase* m = ModelData::ModelDataFactory::createModel(type);
		if (m != nullptr)
		{
			m->generateParaInfo();
			m->setName(name);
			ModelData::ModelDataSingleton* s = ModelData::ModelDataSingleton::getinstance();
			s->appendModel(m);

			emit _physicsWidget->updatePhysicsTreePySig();
			emit _controlPanel->updateActionStates();
		}
		_pyAgent->unLock();
	}

	void MainWidgetPy::deleteCase(int id)
	{
		_physicsWidget->deleteCase(id);
		_controlPanel->updataPropertyTab(nullptr);
		_pyAgent->unLock();
	}
	void MainWidgetPy::importProject(int id)
	{
		emit _physicsWidget->importProjectSig(id);
		_pyAgent->unLock();
		
	}
	void MainWidgetPy::caseRename(int pid, char* newname)
	{
		QString name = QString(newname);
		_physicsWidget->caseRename(pid, name);
		_pyAgent->unLock();
	}
	void MainWidgetPy::startMesher(char* cmesher)
	{
		QString mesher = QString(cmesher);
		emit _meshWidget->startMesherPySig(mesher);
	}

	void MainWidgetPy::updateComponentSubTree(int id)
	{
		ProjectTree::ProjectTreeBase* tree = _physicsWidget->getProjectByID(id);
		if (tree == nullptr) return;
		ProjectTree::ProjectTreeWithBasicNode* nodetree = dynamic_cast<ProjectTree::ProjectTreeWithBasicNode*>(tree);
		if (nodetree == nullptr) return;
		nodetree->updateComponentSubTree();
		_pyAgent->unLock();
	}
	void MainWidgetPy::updateBCSubTree(int id)
	{
		ProjectTree::ProjectTreeBase* tree = _physicsWidget->getProjectByID(id);
		ProjectTree::ProjectTreeWithBasicNode* nodetree = dynamic_cast<ProjectTree::ProjectTreeWithBasicNode*>(tree);
		if (nodetree == nullptr) return;
		nodetree->updateBCSubTree();
		_pyAgent->unLock();
	}

	void MainWidgetPy::updatePostTree(int id)
	{
		ProjectTree::ProjectTreeBase* tree = _physicsWidget->getProjectByID(id);
		ProjectTree::ProjectTreeWithBasicNode* nodetree = dynamic_cast<ProjectTree::ProjectTreeWithBasicNode*>(tree);
		if (nodetree == nullptr) return;
		nodetree->updatePostTree();
		_pyAgent->unLock();
	}

	void MainWidgetPy::viewCounter(int id, char* variable)
	{
		ProjectTree::ProjectTreeBase* tree = _physicsWidget->getProjectByID(id);
		ProjectTree::ProjectTreeWithBasicNode* nodetree = dynamic_cast<ProjectTree::ProjectTreeWithBasicNode*>(tree);
		if (nodetree == nullptr) return;
		QString va(variable);
		nodetree->viewCounterPost(va);
		_pyAgent->unLock();
	}
	void MainWidgetPy::viewVector(int id, char* variable)
	{
		ProjectTree::ProjectTreeBase* tree = _physicsWidget->getProjectByID(id);
		ProjectTree::ProjectTreeWithBasicNode* nodetree = dynamic_cast<ProjectTree::ProjectTreeWithBasicNode*>(tree);
		if (nodetree == nullptr) return;
		QString va(variable);
		nodetree->viewVectorPost(va);
		_pyAgent->unLock();
	}

	void MainWidgetPy::loadFromMaterialLib(char* namelist)
	{

		QHash<QString, Material::Material*> malib;
		QString path = qApp->applicationDirPath() + "/MaterialLib.xml";

		QFile lib(path);
		if (!lib.open(QIODevice::Text | QIODevice::ReadOnly)) return;

		QDomDocument doc;
		if (!doc.setContent(&lib))
		{
			return;
		}
		QDomNodeList materiallist = doc.elementsByTagName("Material");
		const int nm = materiallist.size();
		for (int i = 0; i < materiallist.size(); ++i)
		{
			QDomElement ele = materiallist.at(i).toElement();
			Material::Material* m = new Material::Material(false);
			m->readDataFromProjectFile(&ele);
			QString name = m->getName();
			malib[name] = m;
		}
		lib.close();

		QString namestr = QString(namelist);
		QStringList nlist = namestr.simplified().split(";");
		const int n = nlist.size();
		if (n <= 0) return;
		Material::MaterialSingleton* s = Material:: MaterialSingleton::getInstance();
		for (int i = 0; i < n; ++i)
		{
			QString ms = nlist.at(i);
			Material::Material* ori = malib.value(ms);
			if (ori == nullptr)  continue;
			for (int i = 0; i < s->getMaterialCount(); i++){
				if (ori->getName() == s->getMaterialAt(i)->getName())
				{
					QMessageBox::warning(NULL, QObject::tr("Warning"),QObject::tr("The material that you want to load from lib already exists!"));
					return;
				}
			}
			Material::Material* nma = new Material::Material;
			nma->copy(ori);
			s->appendMaterial(nma);
		}
		_physicsWidget->updateMaterialTree();
		_pyAgent->unLock();

	}

	void MainWidgetPy::CreateMaterial(char* name,char* type)
	{
		QString sname=QString(name);
		QString stype=QString(type);

		Material::Material* material = nullptr;
		auto ma = ConfigOption::ConfigOption::getInstance()->getMaterialConfig()->getMaterialByType(stype);
		if (ma != nullptr)
		{
			material = new Material::Material;
			material->copy(ma);
		}
		else
		{
		material = Material::MaterialFactory::createMaterial(stype);
		}

		if (material == nullptr)
		{
			QMessageBox::warning(NULL, QObject::tr("Warning"), QObject::tr("Material create failed!"));
		return;
		}
		material->setName(sname);
		Material::MaterialSingleton::getInstance()->appendMaterial(material);
		_physicsWidget->updateMaterialTree();
		_pyAgent->unLock();

	}

	void MainWidgetPy::RemoveFromMaterialLib(char* namelist)
	{

		QString namestr = QString(namelist);
		QStringList nlist = namestr.simplified().split(",");
		if (nlist.size() < 1) return;

		//¶ÁÈ¡xml
		QHash<QString, Material::Material*> malib;
		QString path = qApp->applicationDirPath() + "/MaterialLib.xml";
		QFile lib(path);
		if (!lib.open(QIODevice::Text | QIODevice::ReadOnly)) return ;
		QDomDocument doc;
		if (!doc.setContent(&lib)) return;
		
		QDomNodeList materiallist = doc.elementsByTagName("Material");
		const int nm = materiallist.size();
		for (int i = 0; i < materiallist.size(); ++i)
		{
			QDomElement ele = materiallist.at(i).toElement();
			Material::Material* m = new Material::Material(false);
			m->readDataFromProjectFile(&ele);
			QString name = m->getName();
			malib[name] = m;
		}
		lib.close();
		for (QString name : nlist)
		{
			auto ma = malib.value(name);
			qDebug() << name;
			if (ma == nullptr) continue;
			malib.remove(name);
			delete ma;
		}
		//Ð´Èëxml
		QFile writelib(path);
		if (!writelib.open(QIODevice::Text | QIODevice::WriteOnly)) return;
		QDomDocument wdoc;
		QDomProcessingInstruction instruction = wdoc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		wdoc.appendChild(instruction);
		QDomElement root = wdoc.createElement("MaterialLib");
		wdoc.appendChild(root);
		QList<Material::Material*> mlist = malib.values();
		for (int i = 0; i < mlist.size(); ++i)
		{
			Material::Material*m = mlist.at(i);
			m->writeToProjectFile(&wdoc, &root);
			delete m;
		}
		QTextStream* stream = new QTextStream(&writelib);
		wdoc.save(*stream, 4);
		writelib.close();
		delete stream;
		_pyAgent->unLock();
	}

	void MainWidgetPy::DeleteMaterial(int id, char* name)
	{
		Material::MaterialSingleton * s = Material::MaterialSingleton::getInstance();
		s->removeMaterialByID(id);


		emit _physicsWidget->updateActionStates();
		_physicsWidget->updateMaterialTree();
		emit _physicsWidget->disPlayProp(nullptr);
		_pyAgent->unLock();
	}

	void MainWidgetPy::AddMaterialToLib(int id, char* name)
	{
		Material::MaterialSingleton * s = Material::MaterialSingleton::getInstance();
		s->appendToMaterialLib(id);
		_pyAgent->unLock();
	}

	void MainWidgetPy::updateGeometrySubTree(int id)
	{
		ProjectTree::ProjectTreeBase* tree = _physicsWidget->getProjectByID(id);
		ProjectTree::ProjectTreeWithBasicNode* nodetree = dynamic_cast<ProjectTree::ProjectTreeWithBasicNode*>(tree);
		if (nodetree == nullptr) return;
		nodetree->updateGeometrySubTree();
		_pyAgent->unLock();
	}

}


void createCase(char*name, char* type)
{
	MainWidget::MainWidgetPy::createCase(name, type);
}

void deleteCase(int id)
{
	MainWidget::MainWidgetPy::deleteCase(id);
}

void startMesher(char* mesher)
{
	MainWidget::MainWidgetPy::startMesher(mesher);
}
void importProject(int id)
{
	MainWidget::MainWidgetPy::importProject(id);
}
void caseRename(int pid, char* newname)
{
	MainWidget::MainWidgetPy::caseRename(pid,newname);
}

void updateComponentSubTree(int id)
{
	MainWidget::MainWidgetPy::updateComponentSubTree(id);
}
void updateBCSubTree(int id)
{
	MainWidget::MainWidgetPy::updateBCSubTree(id);
}
void viewCounter(int id, char* variable)
{
	MainWidget::MainWidgetPy::viewCounter(id, variable);
}
void viewVector(int id, char* variable)
{
	MainWidget::MainWidgetPy::viewVector(id, variable);
}


void updatePostTree(int id)
{
	MainWidget::MainWidgetPy::updatePostTree(id);
}

void MAINWIDGETSAPI updateGeometrySubTree(int id)
{
	MainWidget::MainWidgetPy::updateGeometrySubTree(id);
}

void  loadFromMaterialLib(char* namelist)
{
	
	MainWidget::MainWidgetPy::loadFromMaterialLib(namelist);
}

void MAINWIDGETSAPI CreateMaterial(char* name,char* type)
{
	MainWidget::MainWidgetPy::CreateMaterial(name,type);
}

void MAINWIDGETSAPI RemoveFromMaterialLib(char* namelist)
{
	MainWidget::MainWidgetPy::RemoveFromMaterialLib(namelist);
}

void MAINWIDGETSAPI DeleteMaterial(int id, char* name)
{
	MainWidget::MainWidgetPy::DeleteMaterial(id, name);
}

void MAINWIDGETSAPI AddMaterialToLib(int id, char* name)
{
	MainWidget::MainWidgetPy::AddMaterialToLib(id, name);
}