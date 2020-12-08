#include "GeometryThreadBase.h"
#include "mainWindow/mainWindow.h"
#include "python/PyAgent.h"

namespace Geometry
{
	GeometryThreadBase::GeometryThreadBase(GUI::MainWindow *mw, QList<Geometry::GeometrySet*>& res) : 
		ThreadTask(mw), 
		_result(res)
	{
		connect(this, SIGNAL(showGeometryMessageSig(ModuleBase::Message)), _mainwindow, SIGNAL(printMessageToMessageWindow(ModuleBase::Message)));
		connect(this, SIGNAL(updateGeometryTree()), _mainwindow, SIGNAL(updateGeometryTreeSig()));
		connect(this, SIGNAL(updateActionsStates()), _mainwindow, SIGNAL(updateActionsStatesSig()));
		connect(this, SIGNAL(updatePreGeometryActor()), _mainwindow, SIGNAL(updatePreGeometryActorSig()));
	}

	GeometryThreadBase::~GeometryThreadBase()
	{

	}

	void GeometryThreadBase::defaultGeometryFinished()
	{
		emit updateGeometryTree();
//		emit updatePreGeometryActor();
		Py::PythonAagent::getInstance()->unLock();	
		ModuleBase::ThreadTask::threadTaskFinished();
	}

	QList<GeometrySet*>& GeometryThreadBase::getResult()
	{
		return _result;
	}
}