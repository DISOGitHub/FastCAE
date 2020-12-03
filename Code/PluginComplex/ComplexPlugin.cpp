#include "ComplexPlugin.h"
#include "mainWindow/mainWindow.h"
#include "actionComplexManager.h"
#include <QString>
#include <QApplication>
#include <QTranslator>
#include <assert.h>

void Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* ps)
{
	Plugins::PluginBase* p = new Plugins::ComplexPlugin(m);
	ps->append(p);
}

namespace Plugins
{
	ComplexPlugin::ComplexPlugin(GUI::MainWindow* m) :_mainwindow(m)
	{
		_describe = QString(tr("Combinant"));
		connect(m, SIGNAL(preWindowOpenedSig(MainWidget::PreWindow*)), this, SLOT(preWindowOpened(MainWidget::PreWindow*)));
		connect(m, SIGNAL(closePreWindowSig()), this, SLOT(preWindowClosed()));
	}

	ComplexPlugin::~ComplexPlugin()
	{
		if (_actions != nullptr) delete _actions;
	}

	bool ComplexPlugin::uninstall()
	{
		_actions->release();
		PluginBase::uninstall();
		return true;
	}

	bool ComplexPlugin::install()
	{
		_actions = new ActionComplexManager(_mainwindow, this);
		PluginBase::install();
		return true;
	}

	void ComplexPlugin::preWindowOpened(MainWidget::PreWindow* pw)
	{
		_actions->preWindowOpened(pw);
	}

	void ComplexPlugin::preWindowClosed()
	{
		_actions->preWindowClosed();
	}

	void ComplexPlugin::reTranslate(QString lang)
	{
		auto app = static_cast<QApplication*>(QCoreApplication::instance());
		app->removeTranslator(_translator);
		if (lang.toLower() == "chinese")
		{
			bool ok = _translator->load("://translation/transtrations_zh_CN");
			assert(ok);
			app->installTranslator(_translator);
		}
		_actions->reTranslate();
	}



}