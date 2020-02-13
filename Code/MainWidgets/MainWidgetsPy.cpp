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
#include "python/PyAgent.h"
#include <QDebug>
#include <assert.h>

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
		_physicsWidget->caseRename(pid, newname);
		_pyAgent->unLock();
	}
	void MainWidgetPy::startMesher(char* cmesher)
	{
		QString mesher = QString(cmesher);
		emit _meshWidget->startMesherPySig(mesher);
	}

	void MainWidgetPy::updateMeshSubTree(int id)
	{
 		qDebug() << id;
		ProjectTree::ProjectTreeBase* tree = _physicsWidget->getProjectByID(id);
		if (tree == nullptr) return;
		ProjectTree::ProjectTreeWithBasicNode* nodetree = dynamic_cast<ProjectTree::ProjectTreeWithBasicNode*>(tree);
		if (nodetree == nullptr) return;
		nodetree->updateMeshSubTree();
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

void updateMeshSubTree(int id)
{
	MainWidget::MainWidgetPy::updateMeshSubTree(id);
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
