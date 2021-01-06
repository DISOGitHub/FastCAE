#include "GmshDialogBase.h"
#include "python/PyAgent.h"
#include "mainWindow/mainWindow.h"
#include "LocalField.h"
//#include "PhysicalsGroups.h"

namespace Gmsh
{
	GmshDialogBase::GmshDialogBase(GUI::MainWindow* m, MainWidget::PreWindow* pre)
		: GeoDialogBase(m, pre)
	{
		_pyAgent = Py::PythonAagent::getInstance();
		connect(this, SIGNAL(showDialog(QDialog*)), m, SIGNAL(showDialogSig(QDialog*)));
		connect(this, SIGNAL(highLightMeshKernal(MeshData::MeshKernal*)), m, SIGNAL(highLightKernelSig(MeshData::MeshKernal*)));
	}

	GmshDialogBase::~GmshDialogBase()
	{
		emit setSelectMode((int)ModuleBase::None);

		for (LocalDensity* l : _localDensities)
			delete l;
		_localDensities.clear();
		
	}


	QList<LocalDensity*>* GmshDialogBase::getLocalDesities()
	{
		return &_localDensities;
	}

// 	QList<PhysicalsGroups*>* GmshDialogBase::getPhysicalsGroups()
// 	{
// 		return &_physicalsGroups;
// 	}


	void GmshDialogBase::appendLocalDesities()
	{
		for (LocalDensity* f : _localDensities)
		{
			f->appendLocals(_pyAgent);
		}
	}

// 	void GmshDialogBase::appendPhysicals()
// 	{
// 		for (PhysicalsGroups* p : _physicalsGroups)
// 		{
// 			p->appendPhysicals(_pyAgent);
// 		}
// 	}

}
