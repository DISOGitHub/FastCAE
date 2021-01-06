#include "GmshThreadManager.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/processBar.h"
#include "GmshThread.h"

namespace Gmsh
{

	GmshThreadManager::GmshThreadManager(GUI::MainWindow* mw)
	{
		connect(mw, SIGNAL(stopSolve(QWidget*)), this, SLOT(stopThread(QWidget*)));
		connect(this, SIGNAL(threadStarted(QWidget*)), mw, SIGNAL(addProcessBarSig(QWidget*)));
	}
	void GmshThreadManager::stopThread(QWidget* w)
	{
		GmshThread* t = _threadHash.value(w);
		if (t == nullptr) return;
		_threadHash.remove(w);
		t->stop();
//		delete t;
	}

	void GmshThreadManager::insertThread(QWidget* w, GmshThread* t)
	{
		
		_threadHash.insert(w, t);
		emit threadStarted(w);
		connect(t, SIGNAL(threadFinished(GmshThread*)), this, SLOT(threadFinished(GmshThread*)));
		t->run();

	}

	void GmshThreadManager::threadFinished(GmshThread* t)
	{
		QWidget * w = _threadHash.key(t);
		delete  t;
		_threadHash.remove(w);
		ModuleBase::ProcessBar* bar = dynamic_cast<ModuleBase::ProcessBar*>(w);
		if (bar == nullptr) return;
		bar->setProcessRange(0, 100);
		bar->setProcess(100);
	}

	void GmshThreadManager::stopAll()
	{
		QList<GmshThread*> ts = _threadHash.values();
		for (auto t : ts)
		{
			t->stop();
			delete t;
		}
		_threadHash.clear();
	}

	bool GmshThreadManager::isRuning()
	{
		return !_threadHash.isEmpty();
	}

}