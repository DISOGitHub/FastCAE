#include "GmshDialogBase.h"
#include "python/PyAgent.h"
#include "mainWindow/mainWindow.h"

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
	}

	QList<LocalPoint*>* GmshDialogBase::getLocalPoints()
	{
		return &_localPoints;
	}

	void GmshDialogBase::appendPointSizeFiled()
	{
		for (LocalPoint* p : _localPoints)
		{
			QString  code = QString("gmsher.appendSizeFiled(%1,%2,%3,%4)").arg(p->x).arg(p->y).arg(p->z).arg(p->value);
			_pyAgent->submit( code);
		}
		
	}

}

