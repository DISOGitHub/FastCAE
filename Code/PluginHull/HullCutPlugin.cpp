#include "HullCutPlugin.h"
#include "mainWindow/mainWindow.h"
#include "actionHullCutManager.h"
#include <QString>
#include <QApplication>
#include <QTranslator>
#include <assert.h>

void Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* ps)
{
	Plugins::PluginBase* p = new Plugins::HullCutPlugin(m);
	ps->append(p);
}

namespace Plugins
{
	HullCutPlugin::HullCutPlugin(GUI::MainWindow* m) :_mainwindow(m)
	{
		_describe = QString(tr("ShipPlugin"));
		connect(m, SIGNAL(preWindowOpenedSig(MainWidget::PreWindow*)), this, SLOT(preWindowOpened(MainWidget::PreWindow*)));
		connect(m, SIGNAL(closePreWindowSig()), this, SLOT(preWindowClosed()));
		
	}

	HullCutPlugin::~HullCutPlugin()
	{
		if (_actions != nullptr) delete _actions;
	}

	bool HullCutPlugin::uninstall()
	{
		_actions->release();
		PluginBase::uninstall();
		return true;
	}

	void HullCutPlugin::reTranslate(QString lang)
	{
		auto app = static_cast<QApplication*>(QCoreApplication::instance());
		app->removeTranslator(_translator);
		if (lang.toLower() == "chinese")
		{
			bool ok = _translator->load("://translation/transtrations_zh_CN");
//			assert(ok);
			app->installTranslator(_translator); 
		}
		_actions->reTranslate();
	}


	bool HullCutPlugin::install()
	{
		_actions = new ActionHullManager(_mainwindow, this);
		PluginBase::install();
		return true;
	} 

	void HullCutPlugin::preWindowOpened(MainWidget::PreWindow* pw)
	{
		_actions->preWindowOpened(pw);
	}

	void HullCutPlugin::preWindowClosed()
	{

		_actions->preWindowClosed();
	}

}