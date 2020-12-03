#include "MachineryPlugin.h"
#include "mainWindow/mainWindow.h"
#include "actionManager.h"
#include <QString>

void Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* ps)
{
	Plugins::PluginBase* p = new Plugins::MachineryPlugin(m);
	ps->append(p);
}

namespace Plugins
{
	MachineryPlugin::MachineryPlugin(GUI::MainWindow* m) :_mainwindow(m)
	{
		_describe = QString("Machinery");
		connect(m, SIGNAL(preWindowOpenedSig(MainWidget::PreWindow*)), this, SLOT(preWindowOpened(MainWidget::PreWindow*)));
		connect(m, SIGNAL(closePreWindowSig()), this, SLOT(preWindowClosed()));
	}

	MachineryPlugin::~MachineryPlugin()
	{
		if (_actions != nullptr) delete _actions;
	}

	bool MachineryPlugin::uninstall()
	{
		_actions->release();
		return true;
	}

	bool MachineryPlugin::install()
	{
		_actions = new ActionManager(_mainwindow, this);
		return true;
	}

	void MachineryPlugin::preWindowOpened(MainWidget::PreWindow* pw)
	{
		_actions->preWindowOpened(pw);
	}

	void MachineryPlugin::preWindowClosed()
	{
		_actions->preWindowClosed();
	}



}