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
	}

	GmshDialogBase::~GmshDialogBase()
	{
		emit setSelectMode((int)ModuleBase::None);
		for (LocalPoint* l : _localPoints)
			delete l;
		_localPoints.clear();
		for (LocalField* l : _localFields)
			delete l;
		_localFields.clear();
	}

	QList<LocalPoint*>* GmshDialogBase::getLocalPoints()
	{
		return &_localPoints;
	}

	QList<LocalField*>* GmshDialogBase::getLocalFields()
	{
		return &_localFields;
	}

// 	QList<PhysicalsGroups*>* GmshDialogBase::getPhysicalsGroups()
// 	{
// 		return &_physicalsGroups;
// 	}

	void GmshDialogBase::appendPointSizeFiled()
	{
		for (LocalPoint* p : _localPoints)
		{
			QString  code = QString("gmsher.appendSizeFiled(%1,%2,%3,%4)").arg(p->x).arg(p->y).arg(p->z).arg(p->value);
			_pyAgent->submit(code);
		}

	}

	void GmshDialogBase::appendSizeFields()
	{
		for (LocalField* f : _localFields)
		{
			f->appendFields(_pyAgent);
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
