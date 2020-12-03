#include "GmshModule.h"
#include "python/PyAgent.h"
#include "mainWindow/mainWindow.h"
#include "mainWindow/SubWindowManager.h"
#include "DialogSurfaceMesh.h"
#include "DialogSolidMesh.h"
#include "GmshPy.h"
#include "GmshThread.h"
#include "moduleBase/processBar.h"
#include "GmshThreadManager.h"
#include "DialogFluidMesh.h"

namespace Gmsh
{
	GmshModule* GmshModule::_instance = nullptr; 

	GmshModule::GmshModule(GUI::MainWindow* m)
	{
		_mainwindow = m;
		_pyAgent = Py::PythonAagent::getInstance();
		_threadManager = new GmshThreadManager(m);

		GmshPy::init(this, _mainwindow);

		connect(this, SIGNAL(showDialog(QDialog*)), m, SIGNAL(showDialogSig(QDialog*)));
		connect(this, SIGNAL(updateMeshTree()), m, SIGNAL(updateMeshTreeSig()));
		connect(this, SIGNAL(updateSetTree()), m, SIGNAL(updateSetTreeSig()));
		connect(this, SIGNAL(updateActions()), m, SIGNAL(updateActionsStatesSig()));

		connect(m, SIGNAL(preWindowOpenedSig(MainWidget::PreWindow*)), this, SLOT(preWindowOpened(MainWidget::PreWindow*)));
		connect(m, SIGNAL(closePreWindowSig()), this, SLOT(preWindowClosed()));
		connect(m, SIGNAL(editMeshSig(int, int)), this, SLOT(editMesh(int, int)));

		connect(this, SIGNAL(generateSig(GMshPara*)), this, SLOT(generateSlot(GMshPara*)));

		connect(this, SIGNAL(printMessageToMessageWindow(ModuleBase::Message)), m, SIGNAL(printMessageToMessageWindow(ModuleBase::Message)));
	}

	void GmshModule::exec(int commandType /* = 0 */, MainWidget::PreWindow* pre)
	{
		if (pre == nullptr) return;

		QDialog* dlg = nullptr;
		if (commandType == 1)
		{
			dlg = new SurfaceMeshDialog(_mainwindow, pre);
		}
		else if (commandType == 2)
		{
			dlg = new SolidMeshDialog(_mainwindow, pre);
		}
		else if (commandType == 0)
		{
			dlg = new DialogFluidMesh(_mainwindow, pre);
		}
		if (dlg != nullptr)
			emit showDialog(dlg);
		
			
	}

	GmshModule* GmshModule::getInstance(GUI::MainWindow* m)
	{
		if (_instance == nullptr)
			_instance = new GmshModule(m);
		return _instance;
	}

	void GmshModule::finalize()
	{
		_threadManager->stopAll();
		delete _threadManager;
	}

	void GmshModule::preWindowOpened(MainWidget::PreWindow* p)
	{
		GmshPy::setPreWindow(p);
		_preWindow = p;
	}

	void GmshModule::preWindowClosed()
	{
		GmshPy::setPreWindow(nullptr);
		_preWindow = nullptr;
	}

	void GmshModule::generateSlot(GMshPara* para)
	{
		GmshThread* thread = iniGmshThread(para);
		auto processBar = new ModuleBase::ProcessBar(_mainwindow, tr("Gmsh Working..."));
		_threadManager->insertThread(processBar, thread);
	}

	GmshThread* GmshModule::iniGmshThread(GMshPara* para)
	{
		if (_threadManager->isRuning())
		{
			delete para;
			ModuleBase::Message m(ModuleBase::Error_Message, QString("Gmsh is running !"));
			emit printMessageToMessageWindow(m);
		}


		GmshThread* thread = new GmshThread(_mainwindow, _preWindow, this, para->_dim);
		thread->setPara(para);
		delete para;
		return thread;
	}

	GmshThreadManager* GmshModule::getGmshThreadManager()
	{
		return _threadManager;
	}


	void GmshModule::editMesh(int dim, int index)
	{
		if (dim == 3)
		{
			Gmsh::SolidMeshDialog* solid = new Gmsh::SolidMeshDialog(_mainwindow, _preWindow, index);
			emit showDialog(solid);
		}
		else if (dim == 2)
		{
			Gmsh::SurfaceMeshDialog* surface = new Gmsh::SurfaceMeshDialog(_mainwindow, _preWindow, index);
			emit showDialog(surface);
		}
	}

}
